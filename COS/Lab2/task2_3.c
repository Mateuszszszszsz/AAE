#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int child_loop(int read_fd, int write_fd) {
  while (1) {
    char buf[8000];
    read(read_fd, buf, 8000);

    char *c = buf;
    while (*c) {
      *c = toupper((unsigned char)*c);
      c++;
    }

    write(write_fd, buf, strlen(buf));
  }
}

int main(int argc, char **argv) {
  int pipe_c2p[2]; // child to parent
  int pipe_p2c[2]; // parent to child
  if (pipe(pipe_c2p) == -1 || pipe(pipe_p2c) == -1) {
    fprintf(stderr, "Error while creating a pipe");
    return 1;
  }

  pid_t ret_pid = fork();
  if (ret_pid == 0) {
    close(pipe_p2c[1]);
    close(pipe_c2p[0]);

    child_loop(pipe_p2c[0], pipe_c2p[1]);
  } else {
    close(pipe_p2c[0]);
    close(pipe_c2p[1]);

    char *message;
    size_t message_len;
    char read_msg[8000];

    while (1) {
      memset(read_msg, '\0', 8000);

      printf("Type your message:\n");
      getline(&message, &message_len, stdin);

      write(pipe_p2c[1], message, message_len);

      read(pipe_c2p[0], read_msg, 8000);
      printf("Received message: %s\n", read_msg);
    }

    close(pipe_c2p[1]);
    close(pipe_p2c[0]);
    return 0;
  }
}