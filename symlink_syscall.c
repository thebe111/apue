#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int
main(int argc, char **argv) {
   if (argc != 3) {
      fprintf(stderr, "usage: %s <source> <target>", argv[0]);

      exit(EXIT_FAILURE);
   }

   if (symlink(argv[1], argv[2]) < 0) {
      fprintf(
         stderr,
         "unable to symlink '%s' to '%s': %s\n",
         argv[2], argv[1], strerror(errno)
      );
   }

   return EXIT_SUCCESS;
}

