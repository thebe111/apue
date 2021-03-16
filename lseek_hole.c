#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

#define BIGNUM 10240000

char bufone[] = "abcdefghij";
char buftwo[] = "ABCDEFGHIJ";

int
main() {
   int fd;

   if ((fd = creat("file.hole", S_IRUSR | S_IWUSR)) < 0) {
      perror("[-] creat error");
      return EXIT_FAILURE;
   }

   if (write(fd, bufone, strlen(bufone)) != sizeof(bufone) - 1) {
      perror("[-] error writing bufone");
      return EXIT_FAILURE;
   }

   if (lseek(fd, BIGNUM, SEEK_CUR) == -1) {
      perror("[-] lseek error");
      return EXIT_FAILURE;
   }

   if (write(fd, buftwo, strlen(buftwo)) != sizeof(buftwo) - 1) {
      perror("[-] error writing buftwo");
      return EXIT_FAILURE;
   }

   (void) close(fd);

   return EXIT_SUCCESS;
}
