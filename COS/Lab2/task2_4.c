#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MILLION 1000000
#define HUNDRED_K (MILLION / 10)

void child_work(float *numbers, int child_id, int write_fd) {
  float sum = 0;

  for (int i = 0; i < HUNDRED_K; i++) {
    sum += numbers[child_id * HUNDRED_K + i];
  }

  char formatted_output[32];
  sprintf(formatted_output, "%f", sum);

  int x = write(write_fd, formatted_output, strlen(formatted_output));
  exit(0);
}

int main(int argc, char **argv) {
  srandom(time(NULL));
  float *numbers = malloc(sizeof(float) * MILLION);
  for (int i = 0; i < MILLION; i++) {
    numbers[i] = (((float)random() / (float)RAND_MAX) * 2) - 1;
  }

  int pipe_read_fds[10]; // collect pipe fds to gather results

  pid_t ret_pid;
  for (int i = 0; i < 10; i++) {
    int pipe_fd[2];
    pipe(pipe_fd);
    pipe_read_fds[i] = pipe_fd[0];

    ret_pid = fork();
    if (ret_pid == -1) {
      fprintf(stderr, "Fork error\n");
      return 1;
    }

    if (ret_pid == 0) {
      close(pipe_fd[0]);
      child_work(numbers, i, pipe_fd[1]);
    } else {
      close(pipe_fd[1]);
    }
  }

  float sum = 0;

  for (int child_id = 0; child_id < 10; child_id++) {
    char read_value[32];
    if (read(pipe_read_fds[child_id], read_value, 32) == -1) {
      fprintf(stderr, "Error while reading pipe\n");
    }

    sum += atof(read_value);
  }

  int average = sum / 10;
  printf("Total average: %d\n", average);

  return 0;
}