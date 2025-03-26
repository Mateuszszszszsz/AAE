#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define TASK_QUEUE "/task-queue"
#define RESULT_QUEUE "/result-queue"

typedef struct Task {
  int pid;
  uint8_t task_id;
  int A;
  int B;
} Task;

typedef struct Result {
  int pid;
  uint8_t task_id;
  int sum;
} Result;

pid_t *pids;
int n_pids = 0;

struct mq_attr task_queue_attr = {.mq_flags = O_NONBLOCK, .mq_maxmsg = 10, .mq_msgsize = sizeof(Task), .mq_curmsgs = 0};
struct mq_attr result_queue_attr = {.mq_flags = O_NONBLOCK, .mq_maxmsg = 10, .mq_msgsize = sizeof(Result), .mq_curmsgs = 0};

void shutdown(int signo) {
  for (int i = 0; i < n_pids; i++) {
    kill(pids[i], SIGTERM);
    printf("Killed process %d\n", pids[i]);
  }

  while (waitpid(-1, NULL, 0) > 0)
    ;

  mq_unlink(TASK_QUEUE);
  mq_unlink(RESULT_QUEUE);

  exit(0);
}

void producer_loop(mqd_t task_queue, mqd_t result_queue) {
  int task_id_counter = 0;
  while (1) {
    int a = random() % 101;
    int b = random() % 101;
    Task task = {.pid = getpid(), .task_id = task_id_counter++, .A = a, .B = b};

    if (mq_send(task_queue, (char *)&task, sizeof(task), 0) == -1) {
      perror("Producer: mq_send failed");
      exit(1);
    }

    Result result;
    if (mq_receive(result_queue, (char*)&result, sizeof(result), NULL) == -1) {
      perror("Producer: mq_receive failed");
      exit(1);
    } else {
      printf("Result from task %d:\n", task.task_id);
    printf("\tProducer PID: %d\n", task.pid);
    printf("\tA: %3d, B: %3d, Sum: %3d\n", task.A, task.B, result.sum);
    }
    sleep(1);
  }
}

void consumer_loop(mqd_t task_queue, mqd_t result_queue) {
  while (1) {
    Task task;
    int ret = mq_receive(task_queue, (char *)&task, sizeof(task), NULL);
    if (ret == -1) {
      perror("Consumer: mq_receive failed");
      exit(1);
    }

    int sum = task.A + task.B;
    Result result = {.pid = task.pid, task.task_id, sum};
    ret = mq_send(result_queue, (char*)&result, sizeof(result), 0);
    // printf("Consumer %d processed task %d:\n", getpid(), task.task_id);
    // printf("\tProducer PID: %d\n", task.pid);
    // printf("\tA: %3d, B: %3d, Sum: %3d\n", task.A, task.B, sum);
    sleep(5);
  }
}

int main(int argc, char **argv) {
  if (argc < 2 || (argv[1][0] < 'A' || argv[1][0] > 'D')) {
    printf("Select one of the following scenarios:\n");
    printf("A - one producer, one consumer\n");
    printf("B - one producer, multiple consumers\n");
    printf("C - multiple producers, one consumer\n");
    printf("D - multiple producers, multiple consumers\n");
    return 1;
  }

  mqd_t task_queue = mq_open(TASK_QUEUE, O_CREAT | O_RDWR, 0600, &task_queue_attr);
  if (task_queue == (mqd_t)-1) {
    if (errno == EEXIST) {
      printf("Found existing task queue, closing queue");
      int ret;
      ret = mq_unlink(TASK_QUEUE);
      exit(1);
    } else {
      perror("Failed to open task queue");
      exit(1);
    }
  }

  mqd_t result_queue = mq_open(RESULT_QUEUE, O_CREAT | O_RDWR, 0600, &result_queue_attr);
  if (result_queue == (mqd_t)-1) {
    if (errno == EEXIST) {
      printf("Found existing task queue, closing queue");
      int ret;
      ret = mq_close(task_queue);
      ret = mq_unlink(TASK_QUEUE);
      ret = mq_unlink(RESULT_QUEUE);
      exit(1);
    } else {
      perror("Failed to open task queue");
      exit(1);
    }
  }

  switch (argv[1][0]) {
  case 'A': {
    pids = malloc(sizeof(pid_t *) * 2);

    pids[0] = fork();
    if (pids[0] == 0) {
      producer_loop(task_queue, result_queue);
    } else {
      n_pids++;
    }

    pids[1] = fork();
    if (pids[1] == 0) {
      consumer_loop(task_queue, result_queue);
    } else {
      n_pids++;
    }
    break;
  }
  case 'B': {
    pids = malloc(sizeof(pid_t *) * 4);

    pids[0] = fork();
    if (pids[0] == 0) {
      producer_loop(task_queue, result_queue);
    } else {
      n_pids++;
    }

    for (int i = 0; i < 3; i++) {
      pids[i + 1] = fork();
      if (pids[i + 1] == 0) {
        consumer_loop(task_queue, result_queue);
      } else {
        n_pids++;
      }
    }
    break;
  }

  case 'C': {
    pids = malloc(sizeof(pid_t *) * 4);

    for (int i = 0; i < 3; i++) {
      pids[i + 1] = fork();
      if (pids[i + 1] == 0) {
        producer_loop(task_queue, result_queue);
      } else {
        n_pids++;
      }
    }

    pids[0] = fork();
    if (pids[0] == 0) {
      consumer_loop(task_queue, result_queue);
    } else {
      n_pids++;
    }

    break;
  }
  case 'D': {
    pids = malloc(sizeof(pid_t *) * 4);

    for (int i = 0; i < 3; i++) {
      pids[i + 1] = fork();
      if (pids[i + 1] == 0) {
        producer_loop(task_queue, result_queue);
      } else {
        n_pids++;
      }
    }

    for (int i = 0; i < 3; i++) {
      pids[i + 1] = fork();
      if (pids[i + 1] == 0) {
        consumer_loop(task_queue, result_queue);
      } else {
        n_pids++;
      }
    }
    break;
  }
  default:
    fprintf(stderr, "Not implemented/unknown option");
    exit(1);
  }

  signal(SIGINT, shutdown); // shutdown on Ctrl+C
  printf("Ctrl+C to stop\n");

  while (1) {
    pause();
  }

  exit(0);
}
