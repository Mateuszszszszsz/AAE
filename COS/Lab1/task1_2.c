#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t ret_pid = fork();
  if (ret_pid == -1) {
    fprintf(stderr, "Fork error\n");
    return 1;
  }

  pid_t my_pid = getpid();

  if (ret_pid == 0) {
    printf("PID: %d - I am a child.\n", my_pid);
  } else {
    printf("PID: %d - I am a parent\n", my_pid);
  }

  return 0;
}