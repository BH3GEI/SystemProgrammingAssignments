#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

void file_copy_bufsiz(const char *source, const char *destination){
    char buffer[BUFSIZ];
    ssize_t bytes;

    int sourceFile = open(source, O_RDONLY);
    int destFile = open(destination, O_WRONLY | O_CREAT, 0666);

    while((bytes = read(sourceFile, buffer, BUFSIZ)) > 0){
        write(destFile, buffer, bytes);
    }

    close(sourceFile);
    close(destFile);
}

void file_copy_char(const char *source, const char *destination){
    char buffer;
    ssize_t bytes;

    int sourceFile = open(source, O_RDONLY);
    int destFile = open(destination, O_WRONLY | O_CREAT, 0666);

    while((bytes = read(sourceFile, &buffer, 1)) > 0){
        write(destFile, &buffer, bytes);
    }

    close(sourceFile);
    close(destFile);
}

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    clock_t start = clock();
    file_copy_bufsiz(argv[1], argv[2]);
    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken for file copy (BUFSIZ): %f seconds\n", time_spent);

    start = clock();
    file_copy_char(argv[1], argv[2]);
    end = clock();

    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken for file copy (char): %f seconds\n", time_spent);

    return 0;
}