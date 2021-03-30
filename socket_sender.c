#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define DATA "any message"

int
main(int argc, char **argv) {
   int sock;
   struct sockaddr_un name;

   if (argc != 2) {
      perror("usage: send <socket>");
      exit(EXIT_FAILURE);
   }

   sock = socket(PF_LOCAL, SOCK_DGRAM, 0);

   if (sock < 0) {
      perror("opening datagram scoket");
      exit(EXIT_FAILURE);
   }

   name.sun_family = PF_LOCAL;

   (void) strncpy(name.sun_path, argv[1], sizeof(name.sun_path));

   if (sendto(sock, DATA, sizeof(DATA), 0, (struct sockaddr *) &name, sizeof(struct sockaddr_un)) < 0) {
      perror("sending datagram message");
   }

   close(sock);

   return EXIT_SUCCESS;
}
