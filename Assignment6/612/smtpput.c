#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <regex.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MAX_LINES 1000
#define MAX_LINE_LEN 256

// Global variables for the email and password
char *username = "liyao1119@mails.jlu.edu.cn";
char *password = "daiyina2020";

// Global variables to store the base64 encoded email and password
char *encoded_username = NULL;
char *encoded_password = NULL;


char *mail_msg[MAX_LINES];

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                                'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                                'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                                's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0',
                                '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                '+', '/'};

static int mod_table[] = {0, 2, 1};


void error(const char *err_msg)
{
    perror(err_msg);
    exit(0);
}

// Function to check if a string is a valid email address
int is_valid_email(const char *email) {
    regex_t regex;
    int reti;
    char msgbuf[100];

    // Compile regular expression
    reti = regcomp(&regex, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$", REG_EXTENDED);
    if( reti ){ fprintf(stderr, "Could not compile regex\n"); exit(1); }

    // Execute regular expression
    reti = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex); // Free memory
    if (!reti) {
        return 1;
    } else if (reti == REG_NOMATCH) {
        return 0;
    } else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        exit(1);
    }
}

// Function to check if a string is a valid hostname or IP address
int is_valid_hostname_or_ip(const char *hostname) {
    struct sockaddr_in sa;
    struct hostent* host_info;

    // Try to validate as an IP address
    if (inet_pton(AF_INET, hostname, &(sa.sin_addr)) != 0 || inet_pton(AF_INET6, hostname, &(sa.sin_addr)) != 0)
        return 1;

    // Try to validate as a hostname
    host_info = gethostbyname(hostname);
    return host_info != NULL;
}

// Function to check if a file exists
int is_input_available() {
    fd_set set;
    struct timeval timeout;

    // Initialize the file descriptor set.
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    // Initialize the timeout data structure.
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    // select returns 0 if timeout, 1 if input available, -1 if error.
    return select(FD_SETSIZE, &set, NULL, NULL, &timeout);
}

// Function to read a line from the socket
void read_line(int sockfd, char *buffer) {
    char c;
    int i = 0;
    ssize_t read_result;
    while ((read_result = read(sockfd, &c, 1)) > 0) {
        if (c == '\n') {
            break;
        }
        buffer[i++] = c;
    }
    if (read_result == -1) {
        error("Read error");
    }
    buffer[i] = '\0';
}

// Function to send an SMTP command and wait for a specific response
void smtp_command(int sockfd, char *command, char *response) {
    char buffer[256];
    bzero(buffer, 256);
    snprintf(buffer, sizeof(buffer), "%s\r\n", command);
    ssize_t write_result = write(sockfd, buffer, strlen(buffer));
    if (write_result == -1) {
        error("Write error");
    }
    do {
        bzero(buffer, 256);
        read_line(sockfd, buffer);
        printf("%s\n", buffer);
    } while (strstr(buffer, response) != buffer);
}

// Function to send the data part of the SMTP message
void smtp_data(int sockfd, char **data, int lines, char *response){
    char buffer[256];
    for(int i = 0; i < lines; i++){
        bzero(buffer, 256);
        if(strcmp(data[i], ".") == 0) {
            snprintf(buffer, sizeof(buffer), "..\r\n");
        } else {
            snprintf(buffer, sizeof(buffer), "%s\r\n", data[i]);
        }
        ssize_t write_result = write(sockfd, buffer, strlen(buffer));
        if (write_result == -1) {
            error("Write error");
        }
    }
    smtp_command(sockfd, ".", response);
}

//Function to encode strings into base64
char *base64_encode(const char *data, size_t input_length, size_t *output_length) {
    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(*output_length + 1); // adding 1 to make space for '\0'
    if (encoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    encoded_data[*output_length] = '\0'; // adding null character at the end

    return encoded_data;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
    fprintf(stderr, "Usage: %s host from to\n", argv[0]);
    return 1;
    }

    char *host = argv[1];
    char *from = argv[2];
    char *to = argv[3];

    if (!is_valid_email(from)) {
        fprintf(stderr, "Invalid 'from' email address: %s\n", from);
        return 1;
    }

    if (!is_valid_email(to)) {
        fprintf(stderr, "Invalid 'to' email address: %s\n", to);
        return 1;
    }

    if (!is_valid_hostname_or_ip(host)) {
        fprintf(stderr, "Invalid host: %s\n", host);
        return 1;
    }

    if (!is_input_available()) {
        fprintf(stderr, "No input file provided\n");
        return 1;
    }

    // Add addresses for From and To commands
    char from_line[256];
    char to_line[256];
    snprintf(from_line, sizeof(from_line), "From: %s", from);
    snprintf(to_line, sizeof(to_line), "To: %s", to);
    mail_msg[0] = from_line;
    mail_msg[1] = to_line;

    // Now start reading the email content
    char line[256];
    int i = 2;
    while (fgets(line, sizeof(line), stdin) != NULL && i < MAX_LINES) {
        // Remove the newline character at the end of the line
        line[strcspn(line, "\n")] = 0;
    
        // Add the read line to the email message array
        mail_msg[i] = strdup(line);
        if (mail_msg[i] == NULL) {
            error("Memory allocation error");
        }
        i++;
    }
    int lines = i;

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    size_t output_length;

    portno = 25;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("mails.jlu.edu.cn");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], 
        (char *)&serv_addr.sin_addr.s_addr, 
        server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    encoded_username = base64_encode(username, strlen(username), &output_length);
    encoded_password = base64_encode(password, strlen(password), &output_length);
    
    // Start the SMTP conversation
    smtp_command(sockfd, "EHLO mails.jlu.edu.cn", "250");
    smtp_command(sockfd, "AUTH LOGIN", "333");
    smtp_command(sockfd, encoded_username, "334");
    smtp_command(sockfd, encoded_password, "235");

    // Prepare and send the MAIL FROM and RCPT TO commands
    char mail_from_cmd[256];
    char rcpt_to_cmd[256];
    snprintf(mail_from_cmd, sizeof(mail_from_cmd), "MAIL FROM:<%s>", from);
    snprintf(rcpt_to_cmd, sizeof(rcpt_to_cmd), "RCPT TO:<%s>", to);
    smtp_command(sockfd, mail_from_cmd, "250");
    smtp_command(sockfd, rcpt_to_cmd, "250");
    
    // Start the DATA part of the SMTP message
    smtp_command(sockfd, "DATA", "354");
    smtp_data(sockfd, mail_msg, lines, "250");
    smtp_command(sockfd, "QUIT", "221");
    
    close(sockfd);

    free(encoded_username);
    free(encoded_password);
    
    return 0;
}