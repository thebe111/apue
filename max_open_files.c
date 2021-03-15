#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>

#include <sys/resource.h>
#include <sys/stat.h>

void open_files(int num);

int
main() {
   int openmax;
   struct rlimit rlp;   // rlimit pointer

#ifdef OPEN_MAX // ifdef (if defined) was implementated on compilation time
   printf("[+] OPEN_MAX is defined as %d\n", OPEN_MAX);
#else
   printf("[!] OPEN_MAX is not defined on this platform\n");
#endif

   printf("[+] 'getconf OPEN_MAX' says: ");
   (void) fflush(stdout);   // forces a write of all user-space buffered data
                            // for the given output
   (void) system("getconf OPEN_MAX");   // execute a shell command

   errno = 0;

   if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
      if (errno == 0) {
         fprintf(
            stderr, 
            "[-] sysconf(3) considers _SC_OPEN_MAX unsupported?\n"
         );
      } else {
         fprintf(
            stderr, 
            "[-] sysconf(3) error for _SC_OPEN_MAX: %s\n", 
            strerror(errno)
         );
      }

      exit(EXIT_FAILURE);
   }

   printf("[+] sysconf(3) says this process can open %d files\n", openmax);

   if (getrlimit(RLIMIT_NOFILE, &rlp) != 0) {
      fprintf(
         stderr, 
         "[-] unable to get per process rlimit: %s\n", 
         strerror(errno)
      );

      exit(EXIT_FAILURE);
   }

   openmax = (int) rlp.rlim_cur;
   printf("[+] getrlimit(2) says this process can open %d files\n", openmax);

   printf("[!] which one is it?\n\n");

   open_files(openmax);
   return EXIT_SUCCESS;
}

void open_files(int num) {
   struct stat stats;
   int count = 0;
   int file_descriptor;  // usually represented as fd

   for (int i = 0; i < num; i++) {
      if (fstat(i, &stats) == 0) {
         printf("[+] file descriptor #%d is open\n", i);
         count++;
      }
   }

   printf("[+] currently open files: %d\n", count);

   for (int i = count; i <= num + 1; i++) {
      if ((file_descriptor = open("/dev/null", O_RDONLY)) < 0) {
         if (errno == EMFILE) {
            printf(
               "[+] opened %d additional files, then failed due to limit\n", 
               i - count
            );
            break;
         } else {
            fprintf(
               stderr, 
               "[-] unable to open '/dev/null': %s\n", 
               strerror(errno)
            );
         }
      }
   }
}

