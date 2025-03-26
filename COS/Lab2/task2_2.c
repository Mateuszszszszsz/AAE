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

  pid_t ret_pid = fork();
  if (ret_pid == 0) {
    wait(NULL);

    char read_msg[32];
    read(pipefd[0], read_msg, 32);  // read

    printf("PID: %d - Received message: `%s`\n", getpid(), read_msg);
    return 0;
  } else {
    char message[] = "Hello world!";
    sleep(2);

    printf("PID: %d - Sending message: `%s`\n", getpid(), message);
    write(pipefd[1], message, strlen(message));
    return 0;
  }
}