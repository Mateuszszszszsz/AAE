#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>

void* count(void* arg)
{
  for (int i = 0; i < 10; i++)
  {
    printf("%d\n", i);
    sleep(1);
  }
  return NULL;
}

int main(int argc, char** argv)
{
  pthread_t thread;

  if (pthread_create(&thread, NULL, count, NULL) != 0)
  {
    perror("Failed to create thread");
    return 1;
  }

  if (pthread_join(thread, NULL) != 0)
  {
    perror("Failed to join thread");
    return 1;
  }

  printf("Waiting complete\n");
}