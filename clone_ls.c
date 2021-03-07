/* example copied from Steve's course: https://stevens.netmeister.org/631/ */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include <sys/types.h>

int
main(int argc, char **argv) {
   DIR *input_dir;  // the DIR type was come from dirent.h
   struct dirent *dir;

   if (argc != 2) {
      fprintf(stderr, "usage: %s directory name\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   if ((input_dir = opendir(argv[1])) == NULL ) {
      fprintf(stderr, "can't open '%s'\n", argv[1]);
      exit(EXIT_FAILURE);
   }

   while ((dir = readdir(input_dir)) != NULL) printf("%s\n", dir->d_name);
   closedir(input_dir);
   
   return 0;
}

