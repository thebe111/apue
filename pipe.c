#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int
main() {
   int n, r, fd[2];
   pid_t pid;
   char line[BUFSIZ];

   if (pipe(fd) < 0) {
      perror("pipe err");
      exit(EXIT_FAILURE);
   }

   if ((pid = fork()) < 0) {
      perror("fork err");
      exit(EXIT_FAILURE);
   } else if (pid > 0) {
      close(fd[0]);

      printf(
            "P => parent process with pid %d (and its ppid %d)\n",
            getpid(),
            getppid()
            );

      printf("P => sending a message to the child process (pid %d)\n", pid);

      snprintf(
            line,
            BUFSIZ,
            "hello child, im your parent pid %d\n", getpid()
            );

      if ((r = write(fd[1], line, strlen(line))) < 0) {
         fprintf(stderr, "unable to write to pipe: %s\n", strerror(errno));
      }

      close(fd[1]);
   } else {
      close(fd[1]);

      printf(
            "C => child process with pid %d (and its ppid %d)\n",
            getpid(),
            getppid()
            );

      printf("C => reading a message from the parent (pid %d)\n", getppid());

      n = read(fd[0], line, BUFSIZ);

      close(fd[0]);

      if ((r = write(STDOUT_FILENO, line, n)) < 0) {
         fprintf(
               stderr,
               "unable to write to pipe: %s\n",
               strerror(errno)
               );
      }
   }

   (void) wait(NULL);

   return EXIT_SUCCESS;
}

