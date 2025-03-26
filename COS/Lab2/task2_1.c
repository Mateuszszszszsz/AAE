#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    fprintf(stderr, "Error while creating a pipe");
    return 1;
  }

  char message[] = "Hello world!";

  printf("Sending message: `%s`\n", message);
  write(pipefd[1], message, strlen(message));

  sleep(1);

  char read_msg[32];
  read(pipefd[0], read_msg, 32);
  printf("Received message: `%s`\n", read_msg);

  return 0;
}