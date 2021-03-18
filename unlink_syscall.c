#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void run_df();

int
main() {
   int fd;

   if (chdir("/var/tmp") == -1) {
      perror("[-] unable to cd to /var/tmp");
      exit(EXIT_FAILURE);
   }

   run_df();

   printf("[+] hiw return to create a 500M file");

   (void) getchar();

   if (system("dd if=/dev/zero of=foo bs=1024k count=500 >/dev/null") != 0) {
      perror("[-] unable to dd a new file");
      exit(EXIT_FAILURE);
   }

   run_df();

   printf("\n[+] hit return to link 'bar' to 'foo'...");

   (void) getchar();

   if (link("foo", "bar") == -1) {
      perror("[-] unable to create a second hard link");
      exit(EXIT_FAILURE);
   }

   if (system("ls -li foo bar") != 0) {
      perror("[-] unable to run ls");
      exit(EXIT_FAILURE);
   }

   run_df();

   printf("\n[+] hit return to open and then unlink 'foo'...");

   (void) getchar();

   if ((fd = open("foo", O_RDWR)) < 0) {
      perror("[-] cant open file");
      exit(EXIT_FAILURE);
   }

   if (unlink("foo") < 0) {
      perror("error unlinking");
      exit(EXIT_FAILURE);
   }

   printf("\n[+] ok, foo unlinked. disk space not free did since 'bar' \
still exists...\n");

   /* system will fail here */
   (void) system("ls -li foo bar");

   run_df();

   printf("\n[+] hit return to unlink 'bar'...");
   
   (void) getchar();

   if (unlink("bar") < 0) {
      perror("error unlinking");
      exit(EXIT_FAILURE);
   }

   printf("\n[+] ok, bar unlinked. disk space not free did since i still have \
a file handle open...\n");

   printf("\n[+] running 'ls -li foo bar");

   (void) system("ls -li foo bar");

   run_df();

   printf("\n hit return to close the open file descriptor");

   (void) getchar();

   close(fd);
   printf("\n[+]...and done. disk space is freed now\n");

   run_df();

   exit(EXIT_SUCCESS);
}

void run_df() {
   printf("[+] available space is now:\n");

   if (system("df .") != 0) {
      perror("[-] unable to run df");
      exit(EXIT_FAILURE);
   }
};
