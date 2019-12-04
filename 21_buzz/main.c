#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <unistd.h>

#define SIGTERM_MSG "SIGTERM received.\n"

unsigned long long count = 0;

void quit_buzz(int signum) {
    printf("\nCount:%llu\n", count);
    exit(0);
}


int main() {
    signal(SIGQUIT, quit_buzz);
    while (1) {
        printf("\a");
        fflush(stdout);
        count++;
        sleep(1);
    }
    return 0;
}