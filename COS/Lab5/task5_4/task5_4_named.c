#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_NAME "/shm-counter"
#define SEM_NAME "/counter-sem"

void increment() {
  int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
  int *p_counter = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  sem_t *sem = sem_open(SEM_NAME, O_RDWR);

  for (int i = 0; i < 100000; i++) {
    sem_wait(sem);
    (*p_counter)++;
    sem_post(sem);
  }

  exit(0);
}

int main(int argc, char **argv) {
  pid_t pids[10];

  sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);
  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
  if (shm_fd == -1)
  {
    perror("Error while opening shared memory");
    exit(1);
  }

  ftruncate(shm_fd, sizeof(int));
  int *p_counter = mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
  if (p_counter == (int*)-1)
  {
    perror("Error while mapping memory");
    exit(1);
  }
  *p_counter = 0;

  for (int i = 0; i < 10; i++) {
    if (fork() == 0) {
      increment();
    }
  }

  while (wait(NULL) > 0)
    ;

  printf("Value of counter: %d\n", *p_counter);
  sem_unlink(SEM_NAME);
  shm_unlink(SHM_NAME);
}