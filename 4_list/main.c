#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct vector {
  unsigned int size;
  unsigned int cap;
  char **str_array;
} vector;

vector *newV() {
  vector *v = malloc(sizeof(vector));
  if (v == NULL) {
    perror("malloc");
    exit(errno);
  }
  v->size = 0;
  v->str_array = 0;
  v->cap = 0;
  return v;
}

void addV(vector *v, char *str) {
  int len;
  v->size++;
  if (v->cap <= v->size) {
    v->cap = (v->size) * 2;
    v->str_array = realloc(v->str_array, v->cap * sizeof(char *));
  }
  len = strlen(str) + 1;
  v->str_array[v->size - 1] = malloc(len);
  if (v->str_array[v->size - 1] == NULL) {
    perror("malloc");
    v->size--;
  }
  strcpy(v->str_array[v->size - 1], str);
}
void printV(vector *v) {
  for (int i = 0; i < v->size; ++i) {
    printf("%s", v->str_array[i]);
  }
}

int main() {
  char buffer[4048];
  vector *v = newV();
  while (1) {
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      if(errno)
        perror("stdin");
      break;
    }
    if (buffer[0] == '.')
      break;
    addV(v, buffer);
  }
  printV(v);
  return 0;
}