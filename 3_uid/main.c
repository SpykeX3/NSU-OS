#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  char str[1000];
  printf("%d %d\n", getuid(), geteuid());
  FILE *file = fopen("myfile", "r");
  if (file == NULL) {
    perror("myfile");
  } else {
    fgets(str, sizeof(str), file);
    printf("%s", str);
    fclose(file);
  }
  seteuid(getuid());
  printf("%d %d\n", getuid(), geteuid());
  file = fopen("myfile", "r");
  if (file == NULL) {
    perror("myfile");
  } else {
    fgets(str, sizeof(str), file);
    printf("%s", str);
    fclose(file);
  }
  return 0;
}