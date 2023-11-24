#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int input_fd, output_fd;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        exit(1);
    }

    input_fd = open(argv[1], O_RDONLY);
    if (input_fd < 0) {
        perror("open input file");
        exit(1);
    }

    output_fd = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if (output_fd < 0) {
        perror("open output file");
        close(input_fd);
        exit(1);
    }

    close(0);
    if (dup2(input_fd, 0) < 0) {
        perror("dup2 input file");
        close(input_fd);
        close(output_fd);
        exit(1);
    }
    close(input_fd);

    close(1);
    if (dup2(output_fd, 1) < 0) {
        perror("dup2 output file");
        close(output_fd);
        exit(1);
    }
    close(output_fd);

    execlp("wc", "wc", NULL);

    return EXIT_FAILURE;
}