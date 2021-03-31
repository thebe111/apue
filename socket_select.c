#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int
main() {
   int sock, msg_sock, rval;
   char buf[BUFSIZ], *client_addr;
   struct sockaddr_in server;
   struct timeval to;
   struct sockaddr_in client;
   socklen_t len;
   fd_set ready;

   sock = socket(AF_INET, SOCK_STREAM, 0);

   if (sock < 0) {
      perror("opening stream socket");
      exit(EXIT_FAILURE);
   }

   server.sin_family = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port = 0;

   if (bind(sock, (struct sockaddr *) &server, sizeof(server)) != 0) {
      perror("binding stream socket");
      exit(EXIT_FAILURE);
   }

   len = sizeof(server);

   if (getsockname(sock, (struct sockaddr *) &server, &len) != 0) {
      perror("getting socket name");
      exit(EXIT_FAILURE);
   }

   printf("socket has port #%d\n", ntohs(server.sin_port));

   listen(sock, 5);

   do {
      FD_ZERO(&ready);
      FD_SET(sock, &ready);

      to.tv_sec = 5;
      to.tv_usec = 0;

      if (select(sock + 1, &ready, 0, 0, &to) < 0) {
         perror("select");
         continue;
      }

      if (FD_ISSET(sock, &ready)) {
         len = sizeof(client);

         msg_sock = accept(sock, (struct sockaddr *) &client, &len);

         client_addr = inet_ntoa(client.sin_addr);

         printf("client connection from %s\n", client_addr);

         if (msg_sock == -1) perror("accept");
         else do {
            bzero(buf, sizeof(buf));

            if ((rval = read(msg_sock, buf, BUFSIZ)) < 0) {
               perror("reading stream message");
            } else if (rval == 0) {
               printf("ending connection from %s\n", client_addr);
            } else {
               printf("client (%s) sent: %s\n", client_addr, buf);
            }
         } while (rval > 0);

         close(msg_sock);
      } else printf("do something else\n");
   } while (true);

   return EXIT_SUCCESS;
}
