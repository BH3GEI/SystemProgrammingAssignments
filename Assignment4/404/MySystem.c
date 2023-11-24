#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int mysystem(const char *command) {
    int status;
    pid_t pid = fork();

    if (pid == -1) {
        // fork failed
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // this is the child process
        char *argv[4];
        argv[0] = "sh";
        argv[1] = "-c";
        argv[2] = (char *)command;
        argv[3] = NULL;
        execv("/bin/sh", argv);
        // if we reach this point, execve failed
        perror("execve");
        exit(1);
    } else {
        // this is the parent process, wait for the child to finish
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return 1;
        }
        // return the exit status of the child process
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return 1;
        }
    }
}

int main(void) {
    mysystem("ls *.c | wc");
    return EXIT_SUCCESS;
}