#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MILLION 1000000
#define HUNDRED_K (MILLION / 10)

int main(int argc, char **argv) {
  srandom(time(NULL));
  int *numbers = malloc(sizeof(int) * MILLION);
  for (int i = 0; i < MILLION; i++) {
    numbers[i] = random() % 101;
  }

  pid_t ret_pid;
  int i = 0;
  for (; i < 10; i++) {
    ret_pid = fork();

    if (ret_pid == -1) {
      fprintf(stderr, "Fork error\n");
      return 1;
    }

    if (ret_pid == 0) {
      int sum = 0;

      for (int j = 0; j < HUNDRED_K; j++) {
        sum += numbers[i * HUNDRED_K + j];
      }

      int average = sum / HUNDRED_K;
      printf("Average of [%d;%d] = %d\n", i * HUNDRED_K, (i + 1) * HUNDRED_K - 1, average);
      return average;
    }
  }

  int sum = 0;

  for (int proc = 0; proc < 10; proc++) {
    int return_status = 0;
    wait(&return_status);
    if (!WIFEXITED(return_status)) {
      fprintf(stderr, "One of the child processes did not exit normally\n");
      return 1;
    }

    sum += WEXITSTATUS(return_status);
  }

  int average = sum / 10;
  printf("Total average: %d\n", average);

  return 0;
}