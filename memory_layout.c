#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/shm.h>

#define ARR_SIZE 40000
#define MALLOC_SIZE 100000

char arr[ARR_SIZE];
char *str = "a string";
char *str2;
int num = 10, num2;

void func();
void func2(const char *how);

int
main(int argc, char **argv, char **envp) {
   int sint;
   char *ptr;
   char func_arr[ARR_SIZE];

   printf("main at %lX\n", (unsigned long) &main);
   printf("func at %lX\n", (unsigned long) &func);
   printf("func2 at %lX\n", (unsigned long) &func2);
   printf("argc at %lX\n", (unsigned long) &argc);
   printf("argv at %lX\n", (unsigned long) &argv);
   printf("envp at %lX\n", (unsigned long) &envp);
   printf("\n");

   printf(
         "arr[] (on BSS) from %lX to %lX\n",
         (unsigned long) &arr[0],
         (unsigned long) &arr[ARR_SIZE]
         );

   printf("stack around %lX\n", (unsigned long) &sint);

   printf(
         "func_arr[] (on stack) from %lX to %lX\n",
         (unsigned long) &func_arr[0],
         (unsigned long) &func_arr[ARR_SIZE]
         );

   if ((ptr = malloc(MALLOC_SIZE)) == NULL) {
      fprintf(
            stderr,
            "unable to allocate memory: %s\n",
            strerror(errno)
            );

      exit(EXIT_FAILURE);
   }

   printf(
         "malloced from %lX to %lX\n",
         (unsigned long) ptr,
         (unsigned long) ptr+MALLOC_SIZE
         );

   free(ptr);

   printf("\n");

   func();
   printf("func popped off\n");
   func2("from main");

   return EXIT_SUCCESS;
}

void
func() {
   int funcint;
   
   printf("func: stack frame around %lX\n", (unsigned long) &funcint);
   func2("from func");
}

void
func2(const char *how) {
   int funcint;

   printf("func (%s): stack frame around %lX\n", how, (unsigned long) &funcint);

#ifdef STACKOVERFLOW
   func2("recursive");
#endif
}

