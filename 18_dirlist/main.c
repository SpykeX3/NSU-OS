#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdlib.h>

void printState(struct stat *st) {
    if (S_ISDIR(st->st_mode)) printf("d");
    else if (S_ISREG(st->st_mode)) printf("-");
    else printf("?");
    for (int i = 8; i >= 0; i--) {
        if (st->st_mode & (1 << i)) {
            if (i % 3 == 2)
                printf("r");
            else if (i % 3 == 1)
                printf("w");
            else printf("x");
        } else printf("-");
    }
    printf("   %-5d", st->st_nlink);
}

void printUser(struct stat *st) {
    struct passwd *password = getpwuid(st->st_uid);
    printf("    %-15s", password->pw_name);
    struct group *group = getgrgid(st->st_gid);
    printf("    %-15s", group->gr_name);
}

void printName(char *path) {
    char *name = path;
    for (char *start = path; *start != 0; start++) {
        if (*start == '/' && start[1] != 0) {
            name = start+1;
        }
    }
    printf("   %s\n", name);
}

int main(int argc, char **argv) {
    if (argc < 2)
        printf("	error: No arguments\n");
    for (int i = 1; i < argc; ++i) {
        struct stat st;
        if (stat(argv[i], &st) == -1) {
            printf("%s: %s\n", argv[i], strerror(errno));
            continue;
        }
        printState(&st);
        printUser(&st);
        if (S_ISREG(st.st_mode))
            printf("  %-6ld", st.st_size);
        else printf("  %-6c", ' ');
        char *time = asctime(localtime(&st.st_mtime));
        time[strlen(time) - 1] = 0;
        printf("  %s", time);
        printName(argv[i]);
    }
}