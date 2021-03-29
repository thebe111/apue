#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MSG_SIZE 128

typedef struct msfbug {
   long mtype;
   char mtext[MSG_SIZE];
} msg_buf;

int
main(int argc, char **argv) {
   int msqid;
   key_t key;
   msg_buf rbuf;

   if (argc != 2) {
      fprintf(stderr, "usage: %s <key>\n", argv[0]);

      exit(EXIT_FAILURE);
   }

   if ((key = atoi(argv[1])) < 1) {
      fprintf(stderr, "invaid key: %s\n", argv[1]);

      exit(EXIT_FAILURE);
   }

   if ((msqid = msgget(key, 0666)) < 0) {
      perror("msgget");

      exit(EXIT_FAILURE);
   }

   if (msgrcv(msqid, &rbuf, MSG_SIZE, 1, 0) < 0) {
      perror("msgrcv");

      exit(EXIT_FAILURE);
   }

   printf("%s\n", rbuf.mtext);

   return EXIT_SUCCESS;
}
