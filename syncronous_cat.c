#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFSIZE 512

int
main(int argc, char **argv) {
   int size, flags;
   char buf[BUFFSIZE];

   (void) argc;
   (void) argv;

   if ((flags = fcntl(STDOUT_FILENO, F_GETFL, 0)) < 0) {
      perror("[+] cant get file descriptor flag");
      exit(EXIT_FAILURE);
   }

   flags |= O_SYNC;

   if(fcntl(STDOUT_FILENO, F_SETFL, flags) < 0) {
      perror("[-] cant set file descriptor flags");
      exit(EXIT_FAILURE);
   }

   while ((size = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
      if (write(STDOUT_FILENO, buf, size) != size) {
         fprintf(stderr, "[-] write error\n");
         exit(EXIT_FAILURE);
      }
   }
   if (size < 0) {
      fprintf(stderr, "[-] read error\n");
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
