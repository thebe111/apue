#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STDOUT_MSG "stdout msg\n"
#define STDERR_MSG "stderr msg\n"

void write_both(const char *mark);

int
main() {
   write_both("before dup2\n");

   if (dup2(STDOUT_FILENO, STDERR_FILENO) < 0) {
      perror("unable to redirect stderr to stdout");
      exit(EXIT_FAILURE);
   }

   write_both("after dup2\n");

   return EXIT_SUCCESS;
}

void 
write_both(const char *mark) {
   int len, marklen;

   marklen = strlen(mark);

   if (write(STDOUT_FILENO, mark, marklen) != marklen) {
      perror("unable to write marker to stdout");
      exit(EXIT_FAILURE);
   }

   len = strlen(STDOUT_MSG);

   if (write(STDOUT_FILENO, STDOUT_MSG, len) != len) {
      perror("unable to write to stdout");
      exit(EXIT_FAILURE);
   }

   if (write(STDERR_FILENO, STDERR_MSG, len) != len) {
      perror("unable to write to stderr");
      exit(EXIT_FAILURE);
   }
}

