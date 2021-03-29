#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHM_SIZE 1024

int
main(int argc, char **argv) {
   key_t key;
   int shmid;
   char *data;

   if (argc > 2) {
      fprintf(stderr, "usage: %s <data_to_write>\n", argv[0]);

      exit(EXIT_FAILURE);
   }

   if ((key = ftok("shared_memory.c", 'R')) == -1) {
      perror("ftok");
      exit(EXIT_FAILURE);
   }

   if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
      perror("shmget");

      exit(EXIT_FAILURE);
   }

   data = shmat(shmid, (void *) 0, 0);

   if (data == (char *) (-1)) {
      perror("shmat");

      exit(EXIT_FAILURE);
   }

   if (argc == 2) {
      printf("[ %d ] writing to segment: '%s'\n", getpid(), argv[1]);
      strncpy(data, argv[1], SHM_SIZE);
   } else {
      printf("segment contains: '%s'\n", data);
   }

   if (shmdt(data) == -1) {
      perror("shmdt");

      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}

