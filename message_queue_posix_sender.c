#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <mqueue.h>

#define MQ_PATH "/example_queue"
#define MQ_DEFAULT_PRIORITY 0

int
main(int argc, char **argv) {
   mqd_t mq;

   if ((mq = mq_open(MQ_PATH, O_WRONLY | O_CREAT, 0666, NULL)) == (mqd_t) -1) {
      fprintf(
            stderr,
            "%s: unable to open message queue: %s\n",
            argv[1],
            strerror(errno)
            );

      exit(EXIT_FAILURE);
   }

   for (int i = 1; i < argc; i++) {
      if (mq_send(mq, argv[i], strlen(argv[i]), MQ_DEFAULT_PRIORITY) == -1) {
         fprintf(
               stderr,
               "%s: unable to send message %d: %s\n",
               argv[1],
               i,
               strerror(errno)
               );
      }

#ifndef WAIT
      sleep(1);
#endif
   }

   if (mq_send(mq, "semi-urgent", strlen("semi-urgent"), 5) == -1) {
      fprintf(
            stderr,
            "%s: unable to send urgent message: %s\n",
            argv[1],
            strerror(errno)
            );

      exit(EXIT_FAILURE);
   }

   if (mq_send(mq, "urgent", strlen("urgent"), 10) == -1) {
      fprintf(
            stderr,
            "%s: unable to send urgent message; %s\n",
            argv[1],
            strerror(errno)
            );

      exit(EXIT_FAILURE);
   }

   if (mq_close(mq) == -1) {
      fprintf(
            stderr,
            "%s: unable to close message queue: %s\n",
            argv[1],
            strerror(errno)
            );

      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
