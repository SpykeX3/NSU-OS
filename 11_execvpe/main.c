#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

void execvpe(const char *file, char *const argv[], char *env[]) {
    extern char **environ;
    environ = env;
    execvp(file, argv);
}

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
    char *env[] = {"USER=Vasiliy", "PATH=/bin:/usr/bin", NULL};
    cpargs[argc - 1] = NULL;
    pid_t pid = fork();
    if (!pid) {
        execvpe(argv[1], cpargs, env);
    } else {
        int ret;
        pid_t chld = wait(&ret);
        printf("\nI am the parent!\nMy child returned: %X\n", ret);
        printf("My env is:");
        extern char **environ;
        for (int i = 0; environ[i] != NULL; ++i) {
            printf("%s\n", environ[i]);
        }

    }
    fflush(stdout);
    return 0;
}