#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

// Function to calculate the number of iterations for parent and child process
int calc_iterations(int total, int is_child) {
    return total / 2 + (is_child && total % 2);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [num_chars]\n", argv[0]);
        return 1;
    }

    int total_chars = atoi(argv[1]);
    if (total_chars <= 0) {
        fprintf(stderr, "Error: num_chars must be a positive integer.\n");
        return 1;
    }

    int pipes_ch2par[2], pipes_par2ch[2];
    if (pipe(pipes_ch2par) == -1 || pipe(pipes_par2ch) == -1) {
        perror("Error creating pipe");
        return 1;
    }

    int processID = fork();
    if (processID < 0) {
        perror("Error forking process");
        return 1;
    }

    char counter = 0;
    if (processID == 0) {  // Child process
        int iterations_child = calc_iterations(total_chars, 1);
        while (iterations_child-- > 0) {
            write(pipes_ch2par[1], &counter, sizeof(counter));
            putchar('0' + counter % 10);
            fflush(stdout);
            sleep(1); //let parent process have chance to read
            if (read(pipes_par2ch[0], &counter, sizeof(counter)) <= 0) {
                break;
            }
        }
    } else {  // Parent process
        int iterations_parent = calc_iterations(total_chars, 0);
        while (iterations_parent-- > 0) {
            if (read(pipes_ch2par[0], &counter, sizeof(counter)) <= 0) {
                break;
            }
            putchar('a' + counter % 26);
            fflush(stdout);
            counter++;
            write(pipes_par2ch[1], &counter, sizeof(counter));
            sleep(1); //let child process have chance to read
        }
    }

    return 0;
}