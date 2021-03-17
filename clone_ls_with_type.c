#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>

char *get_type(const struct stat sb);

int
main(int argc, char **argv) {
   DIR *input_dir;
   struct dirent *dir;

   if (argc != 2) {
      fprintf(stderr, "usage: %s dir_name\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   if ((input_dir = opendir(argv[1])) == NULL) {
      fprintf(stderr, "cant open '%s'\n", argv[1]);
      exit(EXIT_FAILURE);
   }

   if (chdir(argv[1]) == -1) {
      fprintf(stderr, "cant chdir to '%s': %s\n", argv[1], strerror(errno));
      return(EXIT_FAILURE);
   }

   while((dir = readdir(input_dir)) != NULL) {
      struct stat sb;
      char *stat_type;

      printf("%s: ", dir->d_name);

      if (stat(dir->d_name, &sb) == -1) {
         fprintf(stderr, "cant stat %s: %s\n", dir->d_name, strerror(errno));
         stat_type = "unknown";
      } else {
         stat_type = get_type(sb);
      }

      if (lstat(dir->d_name, &sb) == -1) {
         fprintf(stderr, "cant lstat %s: %s\n", dir->d_name, strerror(errno));
         continue;
      } else if (S_ISLNK(sb.st_mode)) {
         printf("symlink to ");
      }

      printf("%s\n", stat_type);
   }

   (void) closedir(input_dir);

   return EXIT_SUCCESS;
}

char 
*get_type(const struct stat sb) {
   switch(sb.st_mode & S_IFMT) {
      case S_IFREG:
         return "regular file";
         break;
      case S_IFDIR:
         return "directory";
         break;
      case S_IFBLK:
         return "block special";
         break;
      case S_IFIFO:
         return "FIFO";
         break;
      case S_IFLNK:
         return "symbolic link";
         break;
      case S_IFSOCK:
         return "socket";
         break;
      default:
         return "unknown";
   }
};
