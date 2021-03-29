#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#define MAX_RETRIES 10

#ifndef __DARWIN_UNIX03

union semun {
   int val;
   struct semid_ds *buf;
   ushort *arr;
};

#endif

int init_sem(key_t key);

int
main() {
   key_t key;
   int semid;
   struct sembuf sb;

   sb.sem_num = 0;
   sb.sem_op = -1;
   sb.sem_flg = SEM_UNDO;

   if ((key = ftok("semaphore.c", 'J')) == -1) {
      perror("ftok");

      exit(EXIT_FAILURE);
   }

   if ((semid = init_sem(key)) == -1) {
      perror("initsem");

      exit(EXIT_FAILURE);
   }

   printf("press return to lock");
   (void) getchar();
   printf("trying to lock...\n");

   if (semop(semid, &sb, 1) == -1) {
      perror("semop");

      exit(EXIT_FAILURE);
   }

   printf("locked\n");
   printf("press return to unlock");
   (void) getchar();

   sb.sem_op = 1;

   if (semop(semid, &sb, 1) == -1) {
      perror("semop");

      exit(EXIT_FAILURE);
   }

   printf("unlocked\n");

   return EXIT_SUCCESS;
}

int
init_sem(key_t key) {
   union semun arg;
   struct semid_ds buf;
   struct sembuf sb;
   int semid;

   arg.buf = &buf;
   semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);

   // man semop.2 for struct sembuf details
   if (semid >= 0) {
      sb.sem_num = 0;
      sb.sem_op = 1;
      sb.sem_flg = 0;

      if (semop(semid, &sb, 1) == -1) {
         int err = errno;

         if (semctl(semid, 0, IPC_RMID) < 0) perror("semctl rm");
         errno = err;

         return -1;
      }
   } else if (errno == EEXIST) {
      int ready = 0;
      int err;

      semid = semget(key, 1, 0);

      if (semid < 0) return semid;

      for (int i = 0; i < MAX_RETRIES && !ready; i++) {
         if ((err = semctl(semid, 0, IPC_STAT, arg)) < 0) {
            perror("semctl stat");
            return -1;
         }

         if (arg.buf->sem_otime != 0) ready = 1;
         else sleep(1);
      }

      if (!ready) {
         errno = ETIME;
         
         return -1;
      }
   } else return semid;

   return semid;
}
