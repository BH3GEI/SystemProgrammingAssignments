#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    switch (signum) {
        case SIGSEGV:
            printf("Received SIGSEGV, invalid memory access\n");
            exit(1);
        case SIGFPE:
            printf("Received SIGFPE, division by zero\n");
            exit(1);
        case SIGWINCH:
            printf("Received SIGWINCH, window size changed\n");
            exit(1);
        default:
            printf("Received unknown signal\n");
            exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [0|1|2]\n", argv[0]);
        exit(1);
    }

    signal(SIGSEGV, signal_handler);
    signal(SIGFPE, signal_handler);
    signal(SIGWINCH, signal_handler);

    if (atoi(argv[1]) == 0) {
        // Intentionally cause a division by zero
        int x = 1;
        int y = 0;
        printf("%d", x / y);
    } else if (atoi(argv[1]) == 1) {
        // Intentionally cause a segmentation fault by de-referencing a null pointer
        int *p = NULL;
        printf("%d", *p);
    } else if (atoi(argv[1]) == 2) {
        // Pause and wait for SIGWINCH
        pause();
    }

    return 0;
}