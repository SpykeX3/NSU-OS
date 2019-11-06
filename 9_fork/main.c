#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main() {
    pid_t pid = fork();
    char *pargs[] = {"ls", "/home/spyke/.bashrc", NULL};
    if (!pid) {
        execvp("/bin/cat", pargs);
    } else {
        int ret;
        pid_t chld = wait(&ret);
        printf("\nI am the parent!\nMy child %d returned: %X\n", chld, ret);
    }
    fflush(stdout);
    return 0;
}