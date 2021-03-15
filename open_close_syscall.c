#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int
main() {
   int fd;

   if ((fd = open("/home/gabriel/notes/qemu.md", O_RDONLY)) < 0) {
      fprintf(stderr, "[-] error to open file: %s\n", strerror(errno));

      exit(EXIT_FAILURE);
   }

   (void) close(fd);

   return EXIT_SUCCESS;
}
