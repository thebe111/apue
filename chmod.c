#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

int
main() {
   struct stat sbuf;

   if (stat("file", &sbuf) == -1) {
      perror("[-] cant stat file");
      exit(EXIT_FAILURE);
   }

   if (chmod("file", (sbuf.st_mode & ~S_IRUSR) | S_ISGID) == -1) {
      perror("[-] cant chmod file");
      exit(EXIT_FAILURE);
   }

   if (chmod("file1", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
      perror("[-] cant chmod file1");
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
