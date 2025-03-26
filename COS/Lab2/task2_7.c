#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int pipe_fd[2];
  pipe(pipe_fd);

  pid_t ret_pid = fork();

  if (ret_pid == 0) {
    close(pipe_fd[0]);

    dup2(pipe_fd[1], 1);
    printf("Hello world!");
  } else {
    close(pipe_fd[1]);

    dup2(pipe_fd[0], 0); // bonus - same thing for parent's stdin

    char buf[16];
    fgets(buf, 16, stdin);

    printf("Received message: `%s`\n", buf);
  }
  return 0;
}