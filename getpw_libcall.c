#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <pwd.h>
#include <assert.h>
#include <limits.h>

struct passwd *who(char *user);

void print_all_entries(char **argv);
void print_one_struct(const struct passwd *pw);

int
main(int argc, char **argv) {
   if(argc > 2) {
      fprintf(stderr, "usage: %s [name|uid]\n", argv[1]);
      exit(EXIT_FAILURE);
   }

   if (argc == 2) {
      print_one_struct(who(argv[1]));
   } else {
      print_all_entries(argv);
   }

   return EXIT_SUCCESS;
}

struct passwd *who(char *user) {
   struct passwd *pw;
   long id;
   char *end_pointer;

   if ((pw = getpwnam(user)) != NULL) return pw;

   id = strtol(user, &end_pointer, 10);
   if (*user && !*end_pointer && (pw = getpwuid(id))) return pw;

   errx(EXIT_FAILURE, "%s: no such user", user);

   return NULL;
}

void
print_all_entries(char **argv) {
   struct passwd *pw;
   errno = 0;

   while ((pw = getpwent()) != NULL) print_one_struct(pw);

   if(errno) {
      fprintf(
         stderr,
         "%s: unable to call getpwent(3): %s\n",
         argv[1], strerror(errno)
      );

      exit(EXIT_FAILURE);
   }

   endpwent();
}

void
print_one_struct(const struct passwd *pw) {
   assert(pw);

   printf(
      "%s:%s:%d:%d:%s:%s:%s\n",
      pw->pw_name,
      pw->pw_passwd,
      pw->pw_uid,
      pw->pw_gid,
      pw->pw_gecos,
      pw->pw_dir,
      pw->pw_shell
   );
}
