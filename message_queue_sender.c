#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MSG_SIZE 128

typedef struct msgbuf {
   long mtype;
   char mtext[MSG_SIZE];
} msg_buf;

int
main(int argc, char **argv) {
   int msqid;
   int msgflg = IPC_CREAT | 0666;
   key_t key;
   msg_buf sbuf;
   size_t buf_len;

   if (argc != 2) {
      fprintf(stderr, "usage: %s message\n", argv[0]);

      exit(EXIT_FAILURE);
   }

   if ((key = atoi(argv[1])) < -1) {
      fprintf(stderr, "invalid key: %s\n", argv[1]);

      exit(EXIT_FAILURE);
   }

   if ((msqid = msgget(key, msgflg)) < 0) {
      perror("msgget");

      exit(EXIT_FAILURE);
   }

   sbuf.mtype = 1;

   strncpy(sbuf.mtext, "queue MESSAGE", MSG_SIZE);

   buf_len = strlen(sbuf.mtext) + 1;

   if (msgsnd(msqid, &sbuf, buf_len, IPC_NOWAIT) < 0) {
      fprintf(
            stderr,
            "%d, %ld, %s, %d\n",
            msqid,
            sbuf.mtype,
            sbuf.mtext,
            (int) buf_len
            );

      perror("msgsnd");

      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
