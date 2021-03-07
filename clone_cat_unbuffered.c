/* example copied from Steve's course: https://stevens.netmeister.org/631/ */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 32768

int
main(int argc, char **argv) {
   int size;
   char buffer[BUFFSIZE];

   /* cast to void to silence compiler warnings */
   (void) argc;
   (void) argv;

   while((size = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0) {
      if(write(STDOUT_FILENO, buffer, size) != size) {
         fprintf(stderr, "unable to write: %s\n", strerror(errno));
         exit(EXIT_FAILURE);
      }
   }

   if(size < 0) {
      fprintf(stderr, "unable to read: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}

