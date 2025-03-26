#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int pipe_fd[2];
  pipe(pipe_fd);

  pid_t ret_pid = fork();

  if (ret_pid == 0) {
    close(pipe_fd[0]);

    dup2(pipe_fd[1], 1);

    execv("/usr/bin/cal", NULL);
  } else {
    close(pipe_fd[1]);

    dup2(pipe_fd[0], 0); // bonus - same thing for parent's stdin

    char buf[8000];
    read(0, buf, 8000);

    printf("This is output of child process intercepted by parent process:\n");
    printf("%s\n", buf);
  }
  return 0;
}