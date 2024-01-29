/*
*	author : Mirco Palese
*	program name : 
*	last edit :	
*	description : 
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/sem.h>
#include "../../include/apue.h"
#include <fcntl.h>


static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);

static int sem_id;

int main (int argc , char *argv[]){

	struct flock fl;
	pid_t pid;
	int fd;
	
	memset(&fl, 0, sizeof(fl));

	/* apre in lettura e scrittura */
	if ( (fd = open("./loc.txt", O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) err_sys("file open error: ");
	

	// lock in eclusive write lock
	fl.l_type = F_WRLCK;

	// lock entire file
	fl.l_whence = SEEK_SET; // offset base is start of the file
	fl.l_start = 0;         // starting offset is zero
	fl.l_len = 0;           // len is zero, which is a special value representing end
							// of file (no matter how large the file grows in future)
	
	fl.l_pid = 0; 			// F_SETLK(W) ignores it; F_OFD_SETLK(W) requires it to be zero
	
	
	if(argc != 3){
		printf("usage: a 'pid' 'color (1 = red 2 = green)' \n");
		err_quit("too few argument\n");
	}
		 	
	pid = atoi(argv[1]);
 	int color = atoi(argv[2]);
	
	
	for(;;){
		/* F_SETLKW specifies blocking mode */
		if (fcntl(fd, F_SETLKW, &fl) == -1) err_sys("fcntl lock file error ");
		//printf("\033[0;33m");	/* yellow */
		if(color == 1){
			kill(pid,SIGUSR1);
			//printf("SIGUSR1\n");
		}	
		else if (color == 2){
			kill(pid,SIGUSR2);
			//printf("SIGUSR2\n");
		}
		//sleep(1);
		fl.l_type = F_UNLCK;
		if (fcntl(fd, F_SETLK, &fl) == -1) err_sys("remove lock error\n");
		
	}
	
	
	
	return 0;
	
}	
	
	
	
	