#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


typedef struct list {
    struct list *prev;
    char *str;
} list;

char *pop(list **ls) {
    if (*ls == NULL) {
        return NULL;
    }
    list *prev = (*ls)->prev;
    char *res = (*ls)->str;
    free(*ls);
    *ls = prev;
    return res;
}

void push(list **ls, char *string) {
    list *new_ls = malloc(sizeof(list));
    if (new_ls == NULL) {
        perror("malloc");
        exit(errno);
    }
    size_t len = strlen(string) + 1;
    new_ls->prev = *ls;
    new_ls->str = malloc(len);
    strcpy(new_ls->str, string);
    *ls = new_ls;
}

void append(list *ls, char *string) {
    size_t new_size = strlen(ls->str) + strlen(string) + 1;
    ls->str = realloc(ls->str, new_size);
    if (ls->str == NULL) {
        perror("realloc");
        return;
    }
    strcat(ls->str, string);
}

void reverse(list **ls) {
    list *new_ls = NULL;
    char *str_tmp;
    while (*ls != NULL) {
        str_tmp = pop(ls);
        push(&new_ls, str_tmp);
    }
    *ls = new_ls;
}

int main() {
    char buffer[16];
    size_t len;
    list *ls = NULL;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            if (errno)
                perror("stdin");
            break;
        }
        if (buffer[0] == '.')
            break;
        push(&ls, buffer);
        len = strlen(buffer);
        while (buffer[len - 1] != '\n' && fgets(buffer, sizeof(buffer), stdin) != NULL) {
            append(ls, buffer);
            len = strlen(buffer);
        }
    }
    reverse(&ls);
    while (ls != NULL) {
        printf("%s", pop(&ls));
    }
    return 0;
}
