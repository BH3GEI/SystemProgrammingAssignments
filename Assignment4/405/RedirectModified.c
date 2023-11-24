#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int open_file(const char* filename, int flags, mode_t mode) {
    int fd = open(filename, flags, mode);
    if (fd < 0) {
        perror("open file");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void redirect_io(int fd, int std_fd) {
    if (close(std_fd) < 0) {
        perror("close std_fd");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, std_fd) < 0) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }
    if (close(fd) < 0) {
        perror("close fd");
        exit(EXIT_FAILURE);
    }
}

void execute_program() {
    if (execlp("wc", "wc", NULL) < 0) {
        perror("execute program");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int input_fd = open_file(argv[1], O_RDONLY, 0);
    int output_fd = open_file(argv[2], O_WRONLY | O_CREAT, 0644);

    redirect_io(input_fd, STDIN_FILENO);
    redirect_io(output_fd, STDOUT_FILENO);

    execute_program();

    return EXIT_FAILURE;
}