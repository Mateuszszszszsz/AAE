#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void print_help() {
  printf("You need to provide 1 argument:\n");
  printf("\t1 - parent finishes execution before child\n");
  printf("\t2 - child finishes execution before parent\n");
}

int main(int argc, char **argv) {
  if (argc < 2) {
    print_help();
    return 1;
  }

  int argument = atoi(argv[1]);

  pid_t ret_pid = fork();
  if (ret_pid == -1) {
    fprintf(stderr, "Fork error\n");
    return 1;
  }

  pid_t my_pid = getpid();

  if (ret_pid == 0) {
    printf("PID: %d - I am a child.\n", my_pid);
    if (argument == 1)
    {
      sleep(10);
    }
  } else {
    printf("PID: %d - I am a parent\n", my_pid);
    if (argument == 2)
    {
      sleep(10);
    }
  }

  return 0;
}