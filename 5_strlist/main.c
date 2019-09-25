#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct vector {
  unsigned int size;
  unsigned int cap;
  off_t *offsets_arr;
} vector;

vector *newV() {
  vector *v = malloc(sizeof(vector));
  if (v == NULL) {
    perror("malloc");
    exit(errno);
  }
  v->size = 0;
  v->offsets_arr = 0;
  v->cap = 0;
  return v;
}

void addV(vector *v, off_t value) {
  int len;
  v->size++;
  if (v->cap <= v->size) {
    v->cap = (v->size) * 2;
    v->offsets_arr = realloc(v->offsets_arr, v->cap * sizeof(off_t *));
    if (v->offsets_arr == NULL) {
      perror("reallocating vector");
      exit(errno);
    }
  }
  v->offsets_arr[v->size - 1] = value;
}
char buffer[1024 * 1024 + 1];

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

  printf("Write line numbers, starting from 1\n");
  read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
  int line_id = atoi(buffer);
  while (line_id) {
    if (line_id >= v->size || line_id < 0) {
      write(STDOUT_FILENO, "Out of range.\n", 14);
    } else {
      off_t target = v->offsets_arr[line_id - 1];
      ssize_t len = v->offsets_arr[line_id] - target;
      lseek(fildes, target, SEEK_SET);
      ssize_t left = len;
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
    read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    line_id = atoi(buffer);
  }
  close(fildes);
  return 0;
}