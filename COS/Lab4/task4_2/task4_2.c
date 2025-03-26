#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define SHM_NAME "/shm_circular_buffer"
#define SAMPLE_RATE 2000

typedef struct {
  int bufsize;
  int index;
  float buffer[];
} CircularBuffer;

sig_atomic_t stop = 0;
pid_t producer_pid, consumer_avg_pid, consumer_f0_pid;

void handle_signal(int sig) {
    stop = 1;
    kill(producer_pid, SIGTERM);
    kill(consumer_avg_pid, SIGTERM);
    kill(consumer_f0_pid, SIGTERM);
    shm_unlink(SHM_NAME);
    exit(0);
}

void producer(int bufsize) {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(CircularBuffer) + bufsize * sizeof(float));
    CircularBuffer *cb = mmap(0, sizeof(CircularBuffer) + bufsize * sizeof(float), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    memset(cb, 0, sizeof(CircularBuffer) + bufsize * sizeof(float));
    cb->bufsize = bufsize;
    cb->index = 0;

    float freq = 100.0;
    float dt = 1.0 / SAMPLE_RATE;
    int t = 0;
    while (!stop) {
        cb->buffer[cb->index] = sin(2 * M_PI * freq * t * dt);
        cb->index = (cb->index + 1) % cb->bufsize;
        t++;
        usleep(dt * 1000000.0f);
    }

    shm_unlink(SHM_NAME);
}

void consumer_avg() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    CircularBuffer *cb = mmap(0, sizeof(CircularBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    int bufsize = cb->bufsize;
    cb = mmap(0, sizeof(CircularBuffer) + bufsize * sizeof(float), PROT_READ, MAP_SHARED, shm_fd, 0);

    double alpha = 0.1;
    double ema = 0.0;
    while (!stop) {
        ema = alpha * cb->buffer[(cb->index - 1 + bufsize) % bufsize] + (1 - alpha) * ema;
        printf("[K_AVG] Exponential moving average: %f\n", ema);
        usleep(100000);
    }
}

void consumer_f0() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    CircularBuffer *cb = mmap(0, sizeof(CircularBuffer), PROT_READ, MAP_SHARED, shm_fd, 0);
    int bufsize = cb->bufsize;
    cb = mmap(0, sizeof(CircularBuffer) + bufsize * sizeof(float), PROT_READ, MAP_SHARED, shm_fd, 0);

    while (!stop) {
        sleep(1);
        int zero_crossings = 0;
        for (int i = 1; i < bufsize; i++) {
            if ((cb->buffer[i - 1] < 0 && cb->buffer[i] > 0) || (cb->buffer[i - 1] > 0 && cb->buffer[i] < 0)) {
                zero_crossings++;
            }
        }
        printf("[K_F0] Estimated frequency: %d Hz\n", zero_crossings * (SAMPLE_RATE / bufsize) / 2);
    }
}

void supervisor() {
    signal(SIGINT, handle_signal);

    if ((producer_pid = fork()) == 0) {
        producer(400);
        exit(0);
    }
    if ((consumer_avg_pid = fork()) == 0) {
        consumer_avg();
        exit(0);
    }
    if ((consumer_f0_pid = fork()) == 0) {
        consumer_f0();
        exit(0);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);
    shm_unlink(SHM_NAME);
}

int main() {
    supervisor();
    return 0;
}
