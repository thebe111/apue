#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/stat.h>

#ifndef SLEEP
#define SLEEP 10
#endif

void create_file();
void fail_excl_file_creation();
void fail_open_none_existing_file();
void open_file();
void truncate_file();

int
main() {
   create_file();
   system("ls -l new_file");
   printf("\n");
   sleep(SLEEP);

   create_file();
   system("ls -l new_file");
   printf("\n");
   sleep(SLEEP);

   fail_excl_file_creation();
   printf("\n");
   sleep(SLEEP);

   open_file();
   printf("\n");
   sleep(SLEEP);

   fail_open_none_existing_file();
   printf("\n");
   sleep(SLEEP);

   truncate_file();

   return EXIT_SUCCESS;
}

void create_file() {
   int fd;

   printf("[+] checking if './new_file' exists...\n");
   system("ls -l ./new_file");
   printf("[+] trying to create './new_file' with O_RDONLY | O_CREAT...\n");

   if ((fd = open(
            "./new_file", 
            O_RDONLY | O_CREAT,  // if exists open as read only else create it
            S_IRUSR | S_IWUSR)  // read / write permission
       ) == -1) {
      fprintf(
            stderr, 
            "[-] unable to create './new_file': %s\n", 
            strerror(errno)
            );

      exit(EXIT_FAILURE);
   }

   printf(
         "[+] './new_file' created. File descriptor is: %d\n", 
         fd
         );
}

void
fail_excl_file_creation() {
   int fd;

   printf("[+] checking if './new_file' exists ...\n");
   system("ls -l ./new_file");
   printf("[+] trying to create './new_file' \
with O_RDONLY | O_CREAT | O_EXCL...\n");

   if ((fd = open(
            "./new_file",
            O_RDONLY | O_CREAT | O_EXCL, 
            S_IRUSR | S_IWUSR)
       ) == -1) {
      fprintf(
         stderr,
         "[-] unable to create './new_file': %s\n",
         strerror(errno)
      );
   }

   // will fail
   if (close(fd) == -1) {
      fprintf(stderr, "[-] closing failed: %s\n", strerror(errno));
   }
};

void
fail_open_none_existing_file() {
   int fd;

   printf("[+] trying to open (non-existant) \
'./no_such_file' with O_RDONLY...\n");

   if ((fd = open("./no_such_file", O_RDONLY)) == -1) {
      fprintf(
         stderr,
         "[-] unable to open './no_such_file': %s\n",
         strerror(errno)
      );
   }

   // will fail without any complaint
   (void) close(fd);
};

void
open_file() {
   int fd;

   printf("[+] trying to open './open_close_syscalls_operations.c' with \
O_RDONLY...\n");

   if ((fd = open("./open_close_syscalls_operations.c", O_RDONLY)) == -1) {
      fprintf(
         stderr,
         "[-] unable to open './open_close_syscalls_operations.c': %s\n",
         strerror(errno)
      );

      exit(EXIT_FAILURE);
   }

   printf("[+] './open_close_syscalls_operations' opened. \
File descriptor is: %d\n", fd);

   if (close(fd) == 0) {
      printf("[+] './open_close_syscalls_operations.c' closed again\n");
   }
}

void
truncate_file() {
   int fd;

   system("cp open_close_syscalls_operations.c new_file");
   printf("[+] copied 'open_close_syscalls_operations' to 'new_file'\n");
   system("ls -l new_file");

   printf("[+] trying to open './new_file' with O_RDONLY | O_TRUNC...\n");

   if ((fd = open("./new_file", O_RDONLY | O_TRUNC)) == -1) {
      fprintf(
         stderr,
         "unable to open './new_file': %s\n",
         strerror(errno)
      );

      exit(EXIT_FAILURE);
   }

   printf("[+] './new_file' opened. File descriptor is: %d\n", fd);
   printf("[!] './new_file' trucated -- see 'ls -l new_file'\n");
   system("ls -l new_file");

   (void) close(fd);
}

