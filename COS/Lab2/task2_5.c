#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t ret_pid = fork();
  if (ret_pid == 0) {
    execv("/usr/bin/cal", NULL);
    abort();
  }
  return 0;
}