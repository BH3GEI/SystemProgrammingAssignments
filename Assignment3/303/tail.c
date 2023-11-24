#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <file> <num_lines>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }

    char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_in_memory == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    close(fd);

    int num_lines = atoi(argv[2]);

    int line_count = 0;
    for (int i = sb.st_size - 1; i >= 0; i--) {
        if (file_in_memory[i] == '\n') {
            line_count++;
        }
        if (line_count == num_lines) {
            for (int j = i + 1; j < sb.st_size; j++) {
                putchar(file_in_memory[j]);
            }
            break;
        }
    }

    if (line_count < num_lines) {
        printf("%s", file_in_memory);
    }

    munmap(file_in_memory, sb.st_size);

    return 0;
}