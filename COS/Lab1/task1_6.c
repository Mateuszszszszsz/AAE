#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t ret_pid = fork();
  if (ret_pid == -1) {
    fprintf(stderr, "Fork error\n");
    return 1;
  }

  srandom(time(NULL));

  int a = random() % 11;
  int b = (random() % 11) + 20;

  if (ret_pid == 0) {
    int sum = a + b;
    exit(sum);
  } else {
    int return_status;
    wait(&return_status);
    if (WIFEXITED(return_status)) {
      printf("%d\n", WEXITSTATUS(return_status));
    }
  }

  return 0;
}