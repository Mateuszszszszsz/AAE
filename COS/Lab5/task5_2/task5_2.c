#include <pthread.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int counter = 0;

void *increment(void *arg) {
  for (int i = 0; i < 10000; i++) {
    counter++;
  }
  return NULL;
}

int main(int argc, char **argv) {
  pthread_t threads[10];

  for (int i = 0; i < 10; i++) {

    if (pthread_create(&threads[i], NULL, increment, NULL) != 0) {
      perror("Failed to create thread");
      return 1;
    }
  }

  for (int i = 0; i < 10; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      perror("Failed to join thread");
      return 1;
    }
  }

  printf("Value of counter: %d\n", counter);
}