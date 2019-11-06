#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>




void printhelp(char *filename) {
    printf("Usage:\n%s /bin/cat /etc/timezon\ne", filename);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printhelp(argv[0]);
        return 0;
    }
    char **cpargs = (char **) malloc(sizeof(char *) * (argc));
    for (int i = 0; i < argc - 1; ++i) {
        cpargs[i] = argv[i + 1];
    }
    cpargs[argc - 1] = NULL;
    pid_t pid = fork();
    if (!pid) {
        execvp(argv[1], cpargs);
    } else {
        int ret;
        pid_t chld = wait(&ret);
        printf("\nI am the parent!\nMy child returned: %X\n", ret);
    }
    fflush(stdout);
    return 0;
}