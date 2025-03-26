#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t pid = fork();
  if (pid == -1) {
    fprintf(stderr, "Fork error\n");
    return 1;
  }

  if (pid == 0) {
    printf("I am a child\n");
  } else {
    printf("I am a parent\n");
  }

  return 0;
}