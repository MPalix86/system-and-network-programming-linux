#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include "apue.h"

int green = 0;
int strnumber;
sem_t *sem;


void *addcar(void *arg){
  int carqueue = 0;
  int wtime;
  for(;;){
    while(!green){
      wtime = rand() % 10+1;
      carqueue += 1;
      printf("Strada %d. Auto in arrivo... Auto in coda: %d\n", strnumber, carqueue);
      sleep(wtime);

    }
    carqueue = 0;
    printf("Strada %d. Auto in coda %d\n", strnumber, carqueue);
    while(green){
      printf("Strada %d. Auto in arrivo... Via libera\n", strnumber);
      sleep(wtime);
    }
  }
}

void *deletecar(void *arg){
  for(;;){
    if(sem_trywait(sem) == -1){
      printf("Semaforo %d Rosso\n", strnumber);
    }
    else{
      printf("Semaforo %d Verde\n", strnumber);
      green=1;
      sleep(30);
      green=0;
    }

  }
}


int main(int argc, char* argv[]) {
    char semname[4] = "";
    strnumber = atoi(argv[2]);
    pthread_t th[2];
    sem = sem_open(semname, O_CREAT);
    strcpy(semname, argv[1]);

    int i, c;
    for (i = 0; i < 2; i++) {
        if (i == 1) {
            if (pthread_create(&th[i], NULL, &addcar, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &deletecar, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }

    for (c = 0; c < 2; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    return 0;
}
