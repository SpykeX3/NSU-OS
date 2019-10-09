#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

typedef struct record {
    off_t offset;
    ssize_t len;
} record;
typedef struct vector {
    unsigned int size;
    unsigned int cap;
    record *record_arr;
} vector;

vector *newV() {
    vector *v = malloc(sizeof(vector));
    if (v == NULL) {
        perror("malloc");
        exit(errno);
    }
    v->size = 0;
    v->record_arr = 0;
    v->cap = 0;
    return v;
}

void addV(vector *v, off_t value) {
    int len;
    v->size++;
    if (v->cap <= v->size) {
        v->cap = (v->size) * 2;
        v->record_arr = realloc(v->record_arr, v->cap * sizeof(record));
        if (v->record_arr == NULL) {
            perror("reallocating vector");
            exit(errno);
        }
    }
    v->record_arr[v->size - 1].offset = value;
}

char buffer[128];

int read_timeout() {
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    return select(FD_SETSIZE, &set, NULL, NULL,
                  &timeout);
}

void dump_all(vector *v, int fildes) {
    lseek(fildes, 0, SEEK_SET);
    off_t left = v->record_arr[v->size - 1].offset;
    while (left > sizeof(buffer)) {
        read(fildes, buffer, sizeof(buffer));
        write(STDOUT_FILENO, buffer, sizeof(buffer));
        left -= sizeof(buffer);
    }
    read(fildes, buffer, left);
    write(STDOUT_FILENO, buffer, left);
}


int main() {
    char *filename = "myfile";
    vector *v = newV();
    addV(v, 0);
    int fildes = open(filename, O_RDONLY);
    off_t count = 0;
    ssize_t current = 0;
    if (fildes == -1) {
        perror(filename);
        exit(errno);
    }
    current = read(fildes, buffer, sizeof(buffer) - 1);
    while (current > 0) {
        for (ssize_t i = 0; i < current; ++i) {
            if (buffer[i] == '\n') {
                addV(v, count + i + 1);
            }
        }
        count += current;
        current = read(fildes, buffer, sizeof(buffer) - 1);
    }
    addV(v, count);

    for (int i = 1; i < v->size; ++i) {
        off_t target = v->record_arr[i - 1].offset;
        v->record_arr[i - 1].len = v->record_arr[i].offset - target;
    }
    printf("Write line numbers, starting from 1\n");
    if (!read_timeout()) {
        dump_all(v, fildes);
        return 0;
    }
    read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    int line_id = atoi(buffer);

    while (line_id) {
        if (line_id >= v->size || line_id < 0) {
            write(STDOUT_FILENO, "Out of range.\n", 14);
        } else {
            off_t target = v->record_arr[line_id - 1].offset;
            ssize_t len = v->record_arr[line_id - 1].len;
            lseek(fildes, target, SEEK_SET);
            ssize_t left = len;
            printf("%ld\n", v->record_arr[line_id - 1].len);
            while (left > sizeof(buffer)) {
                read(fildes, buffer, sizeof(buffer));
                write(STDOUT_FILENO, buffer, sizeof(buffer));
                left -= sizeof(buffer);
            }
            read(fildes, buffer, left);
            write(STDOUT_FILENO, buffer, left);
            if (buffer[left - 1] != '\n')
                write(STDOUT_FILENO, "\n", 1);
        }
        if (!read_timeout()) {
            return 0;
        }
        read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        line_id = atoi(buffer);
    }
    close(fildes);
    return 0;
}
