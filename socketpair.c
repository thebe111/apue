#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define D1 "DATA1"
#define D2 "DATA2"

int
main() {
   int sockets[2], child;
   char buf[BUFSIZ];

   if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
      perror("opening stream socket pair");
      exit(EXIT_FAILURE);
   }

   if ((child = fork()) == -1) perror("fork");
   else if (child) {
      close(sockets[0]);

      printf("parent (%d) --> sending: %s\n", getpid(), D1);

      if (write(sockets[1], D1, sizeof(D1)) < 0) {
         perror("writing stream message");
      }

      if (read(sockets[1], buf, BUFSIZ) < 0) {
         perror("reading stream message");
      }

      printf("parent (%d) --> reading: %s\n", getpid(), buf);

      close(sockets[1]);
   } else {
      close(sockets[1]);

      printf("parent (%d) --> sending: %s\n", getpid(), D2);

      if (write(sockets[0], D2, sizeof(D2)) < 0) {
         perror("writing stream message");
      }

      if (read(sockets[0], buf, BUFSIZ) < 0) {
         perror("reading stream message");
      }

      printf("parent (%d) --> reading: %s\n", getpid(), buf);

      close(sockets[0]);
   }

   return EXIT_SUCCESS;
}
