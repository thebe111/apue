#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/param.h>

int
main(int argc, char **argv) {
   char buf[MAXPATHLEN];

   if (argc != 2) {
      fprintf(stderr, "%s: need a directory!\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   if (chdir(argv[1]) == -1) {
      fprintf(stderr, "%s: unable to chdir to %s\n", argv[0], argv[1]);
      exit(EXIT_FAILURE);
   }

   printf("[+] ok, cwd is now: %s\n", getcwd(buf, MAXPATHLEN));

   return EXIT_SUCCESS;
}

