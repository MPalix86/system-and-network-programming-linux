/*
*	author : Mirco Palese
*	program name : speed test
*	last edit :	29/02/2020
*	description : speed test of varius IPC methods
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include "../../include/apue.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h> 
#include <sys/shm.h>
#include <pthread.h>


#define PLENGHT 60 	/* number of bytes for each pack */
#define PNUM 17500	/* number of packages to send */
					/* PNUM * PLENGHT =~ bytenum, in this case 60 byte *17500 packets  =~ 1 MB */
					
					
					
char* gen_pack();
void print_millis(struct timeval tm1, struct timeval tm2, char *text);

int main (int argc , char *argv[]){
	
	pid_t pid;
	char *packets[PNUM];
	struct timeval tm1,tm2;
	
	char *pack = gen_pack();
	
	for (int i = 0; i < PNUM; i++){
		 packets[i] = pack;
	}
	
	printf("this programm send about 1MB data to other precesses to test the IPC methods speed\n");
	
	/* PIPE TEST----------------------------------*/
	int pipe_fd[2];
	
	if (pipe(pipe_fd) == -1)err_sys("pipe error ");
	
	if( (pid = vfork()) == -1 )err_sys("fork error ");
	else if(pid == 0){
		if (execl("./pipe_recv","pipe_recv",(void*)0) == -1) err_sys("exec error ");
		exit(0);
	}
	else if(pid > 0) close(pipe_fd[0]);	/* closing read end of the pipe */
	else err_sys("fork error ");
    	
  	gettimeofday(&tm1, NULL);				/* getting time */
	for(int i = 0;i < PNUM;i++){
		if( write(pipe_fd[1], packets[i],PLENGHT) != PLENGHT) err_sys("write error ");
	}
	gettimeofday(&tm2, NULL);				/* getting time */

 	kill(pid, SIGINT);
 	close(pipe_fd[1]);
	
	print_millis(tm1,tm2,"pipe time ="); /* calculating range */
	/* END PIPE TEST------------------------------*/
	
	
	/* FIFO TEST----------------------------------*/
	int fifo;
	
	if( (pid = vfork()) == -1 )err_sys("fork error ");
	else if(pid == 0){
		if (execl("./fifo_recv","fifo_recv",(void*)0) == -1) err_sys("exec error ");
		exit(0);
	}
	else if (pid == -1 ) err_sys("fork error ");
	
	sleep(1);
	
	if ( (fifo = mkfifo("./fifo",0766)) == -1 ) {
	
			if ( (fifo = open("./fifo", O_WRONLY )) == -1) err_sys("open error ");

	}
	
	gettimeofday(&tm1, NULL);	
	for(int i = 0;i < PNUM;i++){
		if( write(fifo, packets[i],PLENGHT) != PLENGHT) err_sys("write error ");
	}
	gettimeofday(&tm2, NULL);	
	
 	kill(pid, SIGINT);
 	close(fifo);
	
	print_millis(tm1,tm2,"fifo time =");
	/* END FIFO TEST------------------------------*/


	/* SHARED MEMORY TEST-------------------------*/
	char *shmem_ptr;
	int shmem;
	key_t mem_key = ftok(".", 'a');					/* shared m. key */
	size_t size = sizeof(char) * (PLENGHT * PNUM);	/* shared m. size */
	
	/* getting shared m. id */
	if ( (shmem =  shmget(mem_key, size , 0666 | IPC_CREAT )) == -1) err_sys("shared memory error ");
	
	/* getting shared m. pointer */
	if ((shmem_ptr = shmat(shmem, 0, 0)) == (void *)-1) err_sys("shmat error");
	
	/* 
	* copying all packets inside shared memory (follow in italian)
	* si noti che con la shared memory si è scelto un approccio diverso rispetto ai due 
	* metodi precedenti, dato che non si attende che il processo client legga ogni volta ciò che ha 
	* ricevuto prima di spedire i restanti pacchetti, ma questi vengono inseriti tutti insieme 
	* nello spazio di memoria condivisa. Un approccio simile ai precedenti potrebbe essere 
	* implementato tramite qualche forma di sincronizzazione come ad esempio i semafori o i segnali,
	* o meglio ancora un mutex caricato in un'altra area di memoria condivisa.
	* in questo modo di fatto il tempo impiegato per condividere i dati è uguale al tempo 
	* che il processo impiega per caricarli in memoria.
	*
	* si trova l'esempio con il mutex condiviso memoria subito dopo questo.
	*/
	gettimeofday(&tm1, NULL);
	for(int i = 0; i < PNUM; i++){
		for (int j = 0; j < PLENGHT; j++){
			
			/* 
			* in the 2d array all strings are terminated by \0, we need to remove it because 
			* we are putting a 2d array inside a 1d array and we won't have something like
			* aaa\0aaa\0aaa\0 ... But we wont something like aaaaaaaaa\0, because the size
			* of every packet is known.
			*/
			if(packets[i][j] != '\0'){
				*shmem_ptr = packets[i][j];	/* copying the actual value */
				shmem_ptr++;				/* go to the next address of shared memory */	
			}
		}
	}
	*shmem_ptr = '\0';		
	gettimeofday(&tm2, NULL);				/* adding \0 at the end of the string */
	print_millis(tm1,tm2,"shared memory time =");
	

	if( (pid = vfork()) == -1 )err_sys("fork error ");
	else if(pid == 0){
		if (execl("./shmem_recv","shmem_recv",(void*)0) == -1) err_sys("exec error ");
		exit(0);
	}
	else if (pid == -1 ) err_sys("fork error ");
	
	waitpid(pid,NULL,(int)NULL);

	if (shmctl(shmem, IPC_RMID, 0) < 0) err_sys("shmctl error");
	/* SHARED MEMORY TEST END----------------------*/
	
	
	/* SHARED MEMORY TEST MUTEX-------------------*/
	
	/* creating new mutex inside shared memory area */
	void *shmem_mu_ptr;				/* pointer to mutex shared memory */		 
	pthread_mutex_t *mu;			/* pointer to mutex */
	int shmem_mu_id;				/* shared memory mutex id */		
	int mu_err;
	size = sizeof(pthread_mutex_t); /* shared m. mutex size (size was already declared )*/
	
	/* getting shared m. id for mutex*/
	if ( (shmem_mu_id =  shmget((key_t)1111, size , 0666 | IPC_CREAT )) == -1) err_sys("shared memory error ");
	
	/* getting shared m. pointer for mutex */
	if ((shmem_mu_ptr = shmat(shmem_mu_id, 0, 0)) == (void *)-1) err_sys("shmat error");
	
	/* make mu, that expects a memory area made like pthread_mutex_t , to point the shared memory. */
	mu = (pthread_mutex_t *)shmem_mu_ptr;
	
	/* initialize the attribute to prepare mutex to be placed in shared memory */
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
	
	/* initialize mutex */
	pthread_mutex_init(mu,&attr);
	pthread_mutexattr_destroy(&attr);
	
	
	/* creating new condition variable inside shared memory area */
	void *shmem_cond_ptr;			/* pointer to cond shared memory */
	int shmem_cond_id;				/* shared memory cond id */		
	pthread_cond_t *cond;
	size = sizeof(pthread_cond_t); 	/* shared m. cond size (size was already declared )*/
	
	/* getting shared m. id for cond*/
	if ( (shmem_cond_id =  shmget((key_t)2222, size , 0666 | IPC_CREAT )) == -1) err_sys("shared memory error ");
	
	/* getting shared m. pointer for cond */
	if ((shmem_cond_ptr = shmat(shmem_cond_id, 0, 0)) == (void *)-1) err_sys("shmat error");
	
	/* make cond, that expects a memory area made like pthread_cond_t , to point the shared memory. */
	cond = (pthread_cond_t *)shmem_cond_ptr;
	
	
	/* * initialize the attribute to prepare cond to be placed in shared memory */
	pthread_condattr_t attrcond;
	pthread_condattr_init(&attrcond);
	pthread_condattr_setpshared(&attrcond, PTHREAD_PROCESS_SHARED);

	/* Initialise condition. */
	pthread_cond_init(cond, &attrcond);
	pthread_condattr_destroy(&attrcond);

	/* Use the condition. */

	/* Clean up. */
	pthread_condattr_destroy(&attrcond); 
	

	/* 
	* creating new shared memory area to share only PLENGHT (60) byte at time.
	* In this way we can share only one packet at time, and we must synchronize with mutex
	* and condition variable.
	*/
	void *shmem_ptr60;
	int shmem60;
	char *shmem_char;
	size = sizeof(char) * PLENGHT;	 /* shared m. data size (size was already declared )*/
	
	/* getting shared m. id */
	if ( (shmem60 =  shmget(3333, size , 0666 | IPC_CREAT )) == -1) err_sys("shared memory error ");
	
	/* getting shared m. pointer */
	if ((shmem_ptr60 = shmat(shmem60, 0, 0)) == (void *)-1) err_sys("shmat error");
	
	shmem_char = shmem_ptr60;
	
	/* locking mutex before creating new process to avoid child reading void memory area */
	if( (mu_err = pthread_mutex_lock(mu)) != 0 ) err_exit(mu_err,"lock mutex error");
	
	/* calling child */
	if( (pid = vfork()) == -1 )err_sys("fork error");
	else if(pid == 0){
		if (execl("./shmem_recv1","shmem_recv1",(void*)0) == -1) err_sys("exec error");
		exit(0);
	}
	else if (pid == -1 ) err_sys("fork error");
	
	
	/* 
	* copying one packet per time inside shared memory (follow in italian)
	* in questo caso si copia nella memoria condivisa un pacchetto alla volta e ci si 
	* sincronizza tra processi con un 'altra area di memoria condivisa dove è contenuto un mutex
	*
	* IMPORTANTE: si noti il funzionamento della CONDITION VARIABLE. 
	* pthread_cond_wait in particolare quando viene chiamata, in maniera atomica rilascia il mutex 
	* e fa si che il processo si metta in attesa del segnale. Quando il segnale arriva,
	* sempre atomicamente, pthread_cond_wait blocca nuovamente il mutex e fa si che il processo
	* chiamante prosegua con la sua esecuzione.
	*/
	gettimeofday(&tm1, NULL);
	int i;
	for( i = 0; i < PNUM; i++){
		strcpy(shmem_char, packets[i]);
		if ( (mu_err = pthread_cond_signal(cond)) != 0) err_exit(mu_err,"cond signal error: ");
		printf("data written, iteration number :%d\n",i);
		if ( (mu_err = pthread_cond_wait(cond, mu)) != 0) err_exit(mu_err,"cond wait error"); 
	}
	if ( (mu_err = pthread_mutex_unlock(mu)) != 0) err_exit(mu_err,"mutex unlock error");
	gettimeofday(&tm2, NULL);			
	print_millis(tm1,tm2,"shared memory time with mutex and cond vairable =");
	
	waitpid(pid,NULL,(int)NULL);

	if (shmctl(shmem_mu_id, IPC_RMID, 0) < 0) err_sys("shmctl error");
	if (shmctl(shmem_cond_id, IPC_RMID, 0) < 0) err_sys("shmctl error");
	if (shmctl(shmem60, IPC_RMID, 0) < 0) err_sys("shmctl error");
	/* END SHARED MEMORY TEST---------------------*/
	
	
	return 0;
	
}


char* gen_pack(){

	/* 
	* note that malloc allocate the string on heap, in 
	* this way we can return it from this function.
	* declaring string as char string[PLENGHT] allocate the string in
	* the stack, but the stack is freed when function 
	* terminates, so the memory area returned is undefined;
	*/
	char *string = malloc (PLENGHT); 	
	
	for(int i = 0; i < PLENGHT - 1; i++){
		strcat(string,"a");
	} 
	return string;
}



void print_millis(struct timeval tm1, struct timeval tm2, char *text){
	unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
	
	/* putting calculated time inside string */
	char str[50];
	sprintf(str,"%llu", t);	
	strcat(str, " millisecond\n");
	printf("%s %llu millis\n",text,t);
}




