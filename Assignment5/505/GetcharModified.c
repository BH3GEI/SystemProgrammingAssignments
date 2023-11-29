#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/select.h>
#include <fcntl.h>
#include <bits/sigaction.h>


int timeout_occurred = 0;
int interrupt_occurred = 0;

void alarm_handler(int _) {
    timeout_occurred = 1;
}

void interrupt_handler(int _) {
    interrupt_occurred = 1;
}

int mygetchar(int timeout) {
    struct sigaction sa_alarm, sa_old_alarm, sa_interrupt, sa_old_interrupt;
    memset(&sa_alarm, 0, sizeof(sa_alarm));
    memset(&sa_interrupt, 0, sizeof(sa_interrupt));
    sa_alarm.sa_handler = alarm_handler;
    sa_interrupt.sa_handler = interrupt_handler;
    sigaction(SIGALRM, &sa_alarm, &sa_old_alarm);
    sigaction(SIGINT, &sa_interrupt, &sa_old_interrupt);

    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    int ret = select(1, &read_fds, NULL, NULL, &tv);

    int ch = -1;
    if (ret > 0) {
        char buf;
        char temp;

        if(read(STDIN_FILENO, &buf, 1) > 0) {
            ch = buf;
        }

        while (read(STDIN_FILENO, &temp, 1) > 0 && temp != '\n');
    } else if (ret == 0 || timeout_occurred) {
        ch = -2;
    } else if (interrupt_occurred) {
        ch = -3;
    }

    sigaction(SIGALRM, &sa_old_alarm, NULL);
    sigaction(SIGINT, &sa_old_interrupt, NULL);

    return ch;
}

int main(int argc, char *argv[]) {
    int timeout = 5;
    if (argc > 1) {
        timeout = atoi(argv[1]);
    }
    time_t current_time = time(NULL);
    char *c_time_string = ctime(&current_time);
    printf("Current time: %s\n", c_time_string);
    printf("Please input a character within %d seconds:\n", timeout);
    int ch = mygetchar(timeout);
    current_time = time(NULL);
    c_time_string = ctime(&current_time);
    printf("\nReturn value: %d, current time: %s\n", ch, c_time_string);
    return 0;
}