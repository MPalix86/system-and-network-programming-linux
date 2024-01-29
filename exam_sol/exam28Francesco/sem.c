#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h> 
#include "../../include/apue.h"

int main(int argc, char *argv[]){

  sem_t *sem1, *sem2, *sem3, *sem4;
  char sem[3] = "sem";
  pid_t pid;
  char semname[4] = "";
  int i;
  char ind;

  for (i=1; i<5; i++){

    strcpy(semname,"");
    sprintf(semname, "%s%d", sem, i);
    ind = i + '0';

    printf("%s\n", semname);
    printf("%c\n", ind);

    if((pid = fork()) < 0)
      err_sys("fork error");

    else if(pid == 0){ /*child*/
      if (execl("./street", "street", semname, ind, NULL) < 0)
  			err_sys("execl error");
    }
    else if(pid > 0){

      switch(i){
        case 1:
            sem1=sem_open(semname, O_CREAT, 007, 0);
            break;
        case 2:
            sem2=sem_open(semname, O_CREAT, 007, 0);
            break;
        case 3:
            sem3=sem_open(semname, O_CREAT, 007, 0);
            break;
        case 4:
            sem4=sem_open(semname, O_CREAT, 007, 0);
            break;
        default:
    				printf("No semaphore needs to be added here \n");
    				break;

      }


    }
    sleep(3);
  }
  int quit=0;
  while(!quit)
    {
      if (sem_post(sem1) == -1)
      {
        printf("Sempost error \n");
        exit(-15);
      }
      if (sem_post(sem2) == -1)
      {
        printf("Sempost error \n");
        exit(-15);
      }
      if (sem_post(sem3) == -1)
      {
        printf("Sempost error \n");
        exit(-15);
      }
      if (sem_post(sem4) == -1)
      {
        printf("Sempost error \n");
        exit(-15);
      }
    }
exit(0);
}
