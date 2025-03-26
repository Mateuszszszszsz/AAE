#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int soutfd = dup(1);

  FILE *fake_stdout = fdopen(soutfd, "w");
  fprintf(stdout, "Write to real stdout\n");
  fprintf(fake_stdout, "Write to ''fake'' stdout\n");
}