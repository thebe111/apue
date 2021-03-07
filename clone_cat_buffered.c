/* example copied from Steve's course: https://stevens.netmeister.org/631/ */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char **argv) {
   int size;

   /* cast to void to silence compiler warnings */
   (void) argc;
   (void) argv;

   while((size = getc(stdin)) != EOF) {
      if(putc(size, stdout) == EOF) {
         fprintf(stderr, "unable to write: %s\n", strerror(errno));
         exit(EXIT_FAILURE);
      }
   }

   if(ferror(stdin)) {
      fprintf(stderr, "unable to read: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}

