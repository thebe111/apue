#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define NAME "socket"

int
main() {
   int sock;
   struct sockaddr_un name;
   char buf[BUFSIZ];

   if ((sock = socket(PF_LOCAL, SOCK_DGRAM, 0)) < 0) {
      perror("opening datagram socket");
      exit(EXIT_FAILURE);
   }

   name.sun_family = PF_LOCAL;

   (void) strncpy(name.sun_path, NAME, sizeof(name.sun_path));

   if (bind(sock, (struct sockaddr *) &name, sizeof(struct sockaddr_un))) {
      perror("binding name to datagram socket");
      exit(EXIT_FAILURE);
   }

   printf("name of our socket: '%s'\n", NAME);

   if (read(sock, buf, BUFSIZ) < 0) perror("reading from socket");

   printf("data read from socket: '%s'\n", buf);

   close(sock);

   unlink(NAME);

   return EXIT_SUCCESS;
}
