#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int
main(int argc, char **argv) {
   if (argc != 3) {
      fprintf(stderr, "[-] usage: %s <from> <to>\n", argv[0]);

      exit(EXIT_FAILURE);
   }

   if (rename(argv[1], argv[2]) < 0) {
      fprintf(
         stderr,
         "[-] unable to rename '%s' to '%s': %s\n", 
         argv[1], argv[2], strerror(errno)
      );

      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}

