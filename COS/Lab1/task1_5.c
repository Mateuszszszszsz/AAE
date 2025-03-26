#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t ret_pid = fork();
  if (ret_pid == -1) {
    fprintf(stderr, "Fork error\n");
    return 1;
  }

  if (ret_pid == 0) {
    for (int i = 0; i < 10; i++) {
      sleep(1);
      printf("%d\n", i+1);
    }
    return 0;
  } else {
    int exit_status;
    wait(&exit_status);

    printf("END OF WORK\n");
  }

  return 0;
}