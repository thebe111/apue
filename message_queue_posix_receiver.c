#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <mqueue.h>
#include <sys/stat.h>

#define MQ_PATH "/example_queue"
#define MQ_DEFAULT_PRIORITY 0
#define MQ_FLAGS O_RDONLY | O_CREAT | O_EXCL | O_NONBLOCK
#define MQ_PERMS S_IRUSR | S_IWUSR
#define MQ_MAXMSG 10
#define MQ_MSG_SZ 1024

int rcved_msgs;

void msg_handler();
void empty_queue(mqd_t mq, struct sigevent se);

int
main(int argc, char **argv) {
   struct mq_attr attr;
   struct sigevent se;
   mqd_t mq;

   (void) mq_unlink(MQ_PATH);

   if (signal(SIGUSR1, msg_handler) == SIG_ERR) {
      fprintf(
            stderr,
            "%s: cant catch SIGUSR1: %s",
            argv[0],
            strerror(errno)
            );

      exit(EXIT_FAILURE);
   }

   attr.mq_flags = 0;
   attr.mq_maxmsg = MQ_MAXMSG;
   attr.mq_msgsize = MQ_MSG_SZ;
   attr.mq_curmsgs = 0;

   if ((mq = mq_open(MQ_PATH, MQ_FLAGS, MQ_PERMS, &attr)) == (mqd_t) -1) {
   fprintf(
         stderr,
         "%s: unable to create message queue: %s\n",
         argv[0],
         strerror(errno)
         );

   exit(EXIT_FAILURE);
   }

   se.sigev_notify = SIGEV_SIGNAL;
   se.sigev_signo = SIGUSR1;

   if (mq_notify(mq, &se) == -1) {
   fprintf(
         stderr,
         "%s: unable to set up notification: %s\n",
         argv[0],
         strerror(errno)
         );

   exit(EXIT_FAILURE);
   }

   for(;;) {
      pause();

      if (argc > 1) {
         sleep(5);
      }

      empty_queue(mq, se);
   }

   if (mq_close(mq) == -1) {
      fprintf(
            stderr,
            "%s: unable to close message queue: %s\n",
            argv[0],
            strerror(errno)
            );

      exit(EXIT_FAILURE);
   }

   if (mq_unlink(MQ_PATH) == -1) {
      fprintf(
            stderr,
            "%s: unable to delete message queue: %s\n",
            argv[0],
            strerror(errno));

      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}

void
msg_handler() { rcved_msgs = 1; }

void
empty_queue(mqd_t mq, struct sigevent se) {
   if (rcved_msgs == 1) {
      unsigned prio;
      char buf[BUFSIZ];
      struct mq_attr attr;

      if (mq_getattr(mq, &attr) == -1) {
         fprintf(
               stderr,
               "unable to get mq attributes: %s\n",
               strerror(errno)
               );

         exit(EXIT_FAILURE);
      }

      printf("number of messages in queue: %ld\n", attr.mq_curmsgs);

      while (mq_receive(mq, buf, BUFSIZ, &prio) > 0) {
         printf("message of priority %d: %s\n", prio, buf);
         bzero(buf, BUFSIZ);
      }

      if ((errno > 0) && (errno != EAGAIN)) {
         fprintf(
               stderr,
               "unable to receive message from queue: %s\n",
               strerror(errno)
               );
      }

      if (mq_notify(mq, &se) == -1) {
         fprintf(
               stderr,
               "unable to set up mq notifications: %s\n",
               strerror(errno)
               );

         exit(EXIT_FAILURE);
      }
   }

   rcved_msgs = 0;
}
