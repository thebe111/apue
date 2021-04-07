#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>

int
main(void) {
   int fd, flags;

   if ((fd = open("/tmp/1", O_CREAT | O_RDWR, 0644)) == -1) {
      fprintf(stderr, "unable to open /tmp/1: %s", strerror(errno));
      exit(EXIT_FAILURE);
   }

   if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
      perror("getting file flags");
      exit(EXIT_FAILURE);
   }

   if (flock(fd, LOCK_SH) < 0) {
      perror("flocking");
      exit(EXIT_FAILURE);
   }

   printf("shared lock established - sleeping for 10 seconds\n");

   for (int i = 0; i < 10; i++) {
      fprintf(stderr, ".");
      sleep(1);
   }

   fprintf(stderr, "\n");

   printf("now trying to get an exclusive lock\n");

   for (int i = 0; i < 10; i++) {
      if (flock(fd, LOCK_EX | LOCK_NB) < 0) {
         printf("enable to get an exclusive lock\n");

         if (i == 9) {
            printf("giving up all locks\n");
            flock(fd, LOCK_UN);
            exit(EXIT_FAILURE);
         }

         sleep(1);
      } else {
         printf("exclusive lock established\n");
         break;
      }
   }

   for (int i = 0; i < 10; i++) {
      fprintf(stderr, ".");
      sleep(1);
   }

   fprintf(stderr, "\n");

   return EXIT_SUCCESS;
}

