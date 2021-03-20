#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>
#include <grp.h>
#include <assert.h>

struct passwd *who(char *user);

int
main(int argc, char **argv) {
  gid_t *groups, *glist;
  int counter, maxgroups, ngroups;
  struct passwd *pw;
  struct group *gr;

  if (argc > 2) {
      (void) fprintf(stderr, "[+] usage: %s [name|uid]\n", argv[0]);

      exit(EXIT_FAILURE);
  }

  pw = argv[1] ? who(argv[1]) : NULL;

  if ((maxgroups = sysconf(_SC_NGROUPS_MAX)) < 0) {
     err(EXIT_FAILURE, "[-] unable to call sysconf");
  }

  if ((groups = malloc((maxgroups + 1) * sizeof(gid_t))) == NULL) {
     err(EXIT_FAILURE, "unable to malloc");
  }

  glist = groups;

  if (pw) {
     ngroups = maxgroups;

     if (getgrouplist(pw->pw_name, pw->pw_gid, glist, &ngroups) == -1) {
        if ((glist = malloc(ngroups * sizeof(gid_t))) == NULL) {
            err(EXIT_FAILURE, "[-] unable to malloc");
        }

        (void) getgrouplist(pw->pw_name, pw->pw_gid, glist, &ngroups);
     }
  } else {
   glist[0] = getgid();

   if ((ngroups = getgroups(maxgroups, glist + 1) + 1) == 0) {
      err(EXIT_FAILURE, "[-] unable to getgroups");
   }
  }

  for (counter = 0; counter < ngroups; counter++) {
      if (counter && glist[counter] == glist[0]) continue;

      if ((gr = getgrgid(glist[counter])) == NULL) {
         (void) fprintf(
            stderr,
            "[-] %s: unable to getgrgid(%u): %s\n",
            argv[0], glist[counter], strerror(errno)
         );
      } else {
         (void) printf("%s", gr->gr_name);

         if (counter < ngroups) printf(" ");
      }
  }

  if (counter) printf("\n");
  if (glist != groups) (void) free(glist);
  (void) free(groups);

  return EXIT_SUCCESS;
}

struct passwd *who(char *user) {
   struct passwd *pw;
   long id;
   char *end_pointer;

   if ((pw = getpwnam(user)) != NULL) return pw;

   id = strtol(user, &end_pointer, 10);
   if (*user && !*end_pointer && (pw = getpwuid(id))) return pw;

   errx(EXIT_FAILURE, "[+] %s: no such user", user);

   return NULL;
}
