#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

void myopen(const char *path);

int
main() {
   myopen("file");

   umask(0);

   myopen("file1");

   umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

   myopen("file2");

   return EXIT_SUCCESS;
}

void
myopen(const char *path) {
   int fd;

   if ((fd = open(
         path, 
         O_CREAT | O_EXCL | O_WRONLY,
         S_IRUSR | S_IWUSR | S_IXUSR |
         S_IRGRP | S_IWGRP | S_IXGRP |
         S_IROTH | S_IWOTH | S_IXOTH
   )) == -1) {
      fprintf(
         stderr,
         "[-] unable to create %s: %s\n",
         path, strerror(errno)
      );

      exit(EXIT_FAILURE);
   }

   (void) close(fd);
}
