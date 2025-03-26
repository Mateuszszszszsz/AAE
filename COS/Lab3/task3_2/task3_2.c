#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Please provide queue name in argument:\n/<queue_name>\n");
    return 1;
  }

  if (argv[1][0] != '/') {
    fprintf(stderr, "Queue name has to start with a slash: /<queue_name>\n");
    return 1;
  }

  mqd_t queue = mq_open(argv[1], O_CREAT | O_EXCL, 0600, NULL);
  if (queue == (mqd_t)-1) {
    if (errno == EEXIST) {
      mq_unlink("");
      fprintf(stderr, "Unlinked existing queue: %s\n", argv[1]);

      return 1;
    }

    fprintf(stderr, "Error on mq_open: errno %d\n", errno);
    return 1;
  }

  struct mq_attr queue_attr;
  if (mq_getattr(queue, &queue_attr) == -1) {
    fprintf(stderr, "Error on mq_getattr: errno %d\n", errno);
    mq_close(queue);
    mq_unlink(argv[1]);
    return 1;
  }

  printf("mq_flags: %ld\n", queue_attr.mq_flags);
  printf("mq_maxmsg: %ld\n", queue_attr.mq_maxmsg);
  printf("mq_curmsgs: %ld\n", queue_attr.mq_curmsgs);
  printf("mq_msgsize: %ld\n", queue_attr.mq_msgsize);

  struct mq_attr new_attr = {.mq_flags = O_NONBLOCK,
                             .mq_maxmsg = 32,
                             .mq_curmsgs = 0,
                             .mq_msgsize = 16384}; // everything but O_NONBLOCK will be ignored

  if (mq_setattr(queue, &new_attr, NULL) == -1) {
    fprintf(stderr, "Error on mq_setattr: errno %d\n", errno);
    mq_close(queue);
    mq_unlink(argv[1]);
    return 1;
  }

  if (mq_getattr(queue, &queue_attr) == -1) {
    fprintf(stderr, "Error on mq_getattr: errno %d\n", errno);
    mq_close(queue);
    mq_unlink(argv[1]);
    return 1;
  }

  printf("mq_flags: %ld\n", queue_attr.mq_flags);
  printf("mq_maxmsg: %ld\n", queue_attr.mq_maxmsg);
  printf("mq_curmsgs: %ld\n", queue_attr.mq_curmsgs);
  printf("mq_msgsize: %ld\n", queue_attr.mq_msgsize);

  mq_close(queue);    // closes queue for process
  mq_unlink(argv[1]); // removes queue from system
}