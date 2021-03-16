#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFSIZE 1306
#define COMMENT "\n/* THIS IS MY COMMENT */\n"
#define SOURCE "./read_write_syscall.c"

int
main() {
   int fd, size, len;
   char buf[BUFFSIZE];

   if ((fd = open(SOURCE, O_RDWR | O_APPEND)) == -1) {
      fprintf(
         stderr,
         "[-] unable to open '%s': %s\n",
         SOURCE,
         strerror(errno)
      );

      exit(EXIT_FAILURE);
   }

   if ((size = read(fd, buf, BUFFSIZE)) > 0) {
      if (write(STDOUT_FILENO, buf, size) != size) {
         fprintf(
            stderr,
            "[-] unable to write: %s\n",
            strerror(errno)
         );

         exit(EXIT_FAILURE);
      }
   } else if (size == -1) {
      fprintf(
         stderr,
         "[-] error reading from %s: %s\n",
         SOURCE,
         strerror(errno)
      );

      exit(EXIT_FAILURE);
   }

   printf(
      "\n\n[+] ok, we read the first %d bytes. now lets write something\n",
      BUFFSIZE
   );

   len = sizeof(COMMENT) - 1;

   if (write(fd, COMMENT, len) != len) {
      fprintf(
         stderr,
         "unable to write: %s\n",
         strerror(errno)
      );

      exit(EXIT_FAILURE);
   }

   (void) close(fd);

   return EXIT_SUCCESS;
}

