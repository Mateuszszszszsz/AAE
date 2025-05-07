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

typedef struct shared_mem {
  sem_t sem;
  int counter;
} shared_mem;

void increment() {
  int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
  shared_mem *s_mem = mmap(0, sizeof(shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  sem_t *sem = &s_mem->sem;

  for (int i = 0; i < 100000; i++) {
    sem_wait(sem);
    s_mem->counter++;
    sem_post(sem);
  }

  exit(0);
}

int main(int argc, char **argv) {
  pid_t pids[10];

  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
  if (shm_fd == -1)
  {
    perror("Error while opening shared memory");
    exit(1);
  }

  ftruncate(shm_fd, sizeof(shared_mem));

  shared_mem* s_mem = mmap(0, sizeof(shared_mem), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

  sem_t *sem = &s_mem->sem;
  sem_init(sem, 1, 1);

  int* p_counter = &s_mem->counter;
  if (p_counter == (int*)-1)
  {
    perror("Error while mapping memory");
    shm_unlink(SHM_NAME);
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
