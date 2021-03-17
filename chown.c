#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define GABRIEL 1000
#define USERS_GID 100
#define TTY_GID 5

void mychown(const char *file, int uid, int gid);

int
main(int argc, char **argv) {
   if (argc != 2) {
      fprintf(stderr, "[-] usage: %s file\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   mychown(argv[1], getuid(), -1);
   mychown(argv[1], -1, USERS_GID);
   mychown(argv[1], -1, TTY_GID);

   return EXIT_SUCCESS;
}

void
mychown(const char *file, int uid, int gid) {
   if (chown(file, uid, gid) < 0) {
      fprintf(
         stderr, 
         "[-] unable to chown(%s, %d, %d): %s", 
         file, uid, gid, strerror(errno)
      );
   } else {
      printf("[+] successfully chowned %s to %d:%d\n", file, uid, gid);
   }
}
