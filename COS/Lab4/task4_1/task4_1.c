#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char **argv) {
  char *msg = "Hello";

  int memory = shm_open("/msg", O_RDWR | O_EXCL | O_CREAT, 0600);
  if (memory == -1) {
    if (errno == EEXIST) {
      fprintf(stderr, "Shared memory exists, attempting to close...\n");
      if (shm_unlink("/msg") == -1) {
        perror("Failed to close shared memory\n");
        exit(errno);
      } else {
        printf("Closed shared memory, attempting to reopen...\n");
        memory = shm_open("/msg", O_RDWR | O_EXCL | O_CREAT, 0600);
        if (memory == -1) {
          perror("Failed to reopen memory");
        }
      }
    } else {
      perror("Failed to open shared memory");
      exit(errno);
    }
  }

  ftruncate(memory, strlen(msg));

  if (fork() == 0) {
    char *send_msg = malloc(strlen(msg));
    send_msg = mmap(send_msg, strlen(msg), PROT_WRITE, MAP_SHARED, memory, 0);
    if ((size_t)send_msg == -1) {
      perror("mmap error");
    }

    memcpy(send_msg, msg, strlen(msg));
    shm_unlink("msg");
  } else {
    sleep(3);
    char *received_msg = malloc(strlen(msg));
    received_msg = mmap(received_msg, strlen(msg), PROT_READ, MAP_SHARED, memory, 0);
    printf("%s\n", received_msg);
    shm_unlink("msg");
  }
}