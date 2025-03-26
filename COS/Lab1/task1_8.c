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

int main(int argc, char **argv) {
  srandom(time(NULL));
  float *numbers = malloc(sizeof(float) * MILLION);
  for (int i = 0; i < MILLION; i++) {
    numbers[i] = (((float)random() / (float)RAND_MAX) * 2) - 1;
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
      float sum = 0;

      for (int j = 0; j < HUNDRED_K; j++) {
        sum += numbers[i * HUNDRED_K + j];
      }

      char formatted_output[32];
      sprintf(formatted_output, "%f", sum);

      char filename[16];
      sprintf(filename, "sum%d.txt", getpid());
      long output_file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
      if (output_file == -1) {
        fprintf(stderr, "Error opening file `%s`", filename);
        return 1;
      }

      int bytes_written = write(output_file, formatted_output, strlen(formatted_output));
      if (bytes_written == -1) {
        fprintf(stderr, "Error writing to file `%s`", filename);
        close(output_file);
        return 1;
      }

      close(output_file);
      return 0;
    }
  }

  float sum = 0;

  for (int proc = 0; proc < 10; proc++) {
    int pid = wait(NULL);

    char filename[16];
    sprintf(filename, "sum%d.txt", pid);
    int input_file = open(filename, 'r');
    if (input_file == -1) {
      fprintf(stderr, "Error opening file `%s`", filename);
      return 1;
    }

    char read_value[32];
    read(input_file, read_value, 32);

    sum += atof(read_value);
  }

  int average = sum / 10;
  printf("Total average: %d\n", average);

  return 0;
}