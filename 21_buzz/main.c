#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

unsigned long long count = 0;

void quit_buzz(int signum) {
    printf("\nCount:%llu\n", count);
    exit(0);
}

void main() {
    signal(SIGQUIT, quit_buzz);
    while (1) {
        printf("\a");
        fflush(stdout);
        count++;
        sleep(1);
    }
}