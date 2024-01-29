/*
*	author : Mirco Palese
*	program name : mantissa
*	last edit :	19/02/2020
*	description : leggere compito 26 snp
*	note : non è finito. manca la parte di gestione delle pipe
*/

#include <math.h>                                                               
#include <stdio.h> 
#include <stdlib.h>
#include "../../include/apue.h"
#include <pthread.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <fcntl.h> 


#define A 1      
#define B 1000000000
#define MAX_THREADS 50

void pr_intervals(int intervals[][2], int rows);
void* calc_mantissa(void *intervals);
                                                                                                      
pthread_mutex_t mu ;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
int filedes[2];
pid_t pid;
void exit_handler();

                                                                       
int main(void)                                                                  
{    
	clock_t t1, t2;
	const char *path = "./mantissa_table";
	unsigned char tr_num = 1;
	int err;
	pthread_mutex_init(&mu, NULL);
	
	atexit(exit_handler);
	if(pipe(filedes) == -1) err_sys("pipe error ");
	
	/* creating child */
	if( (pid = vfork()) == -1 )err_sys("fork error ");
	else if(pid == 0){
		if (execl(path,"mantissa_table",&tr_num,(void*)0) == -1) err_sys("execl error ");
		exit(0);
	}
	else sleep(1);
	close(filedes[0]);

	while(tr_num <= MAX_THREADS){
		if( (B % tr_num) == 0){
			t1 = clock();  
			int range = B/tr_num;
			
 			/* 
			* creating one pipe foreach interval 
			* questa parte non è finita in realta viene gestito tutto con un pipe che è 
			* quella creata all'inizio
			*/
// 			int fildes[tr_num][2];
// 			for(int i = 0; i < tr_num; i++){
// 				if(pipe(fildes[i]) == -1) err_sys("pipe error ");
// 			}
	
			
// 			/* close all read end of descriptors */
// 			for(int i = 0; i < tr_num; i+2){
// 				close(fildes[i]);
// 			}
			
			/* calculating intervals */
			int intervals[tr_num][2];
// 			printf("tr_num %d\n", tr_num);
// 			printf("range %d\n", range);
			for(int i = 0; i < tr_num; i++){
				if(i == 0){
					intervals[i][0] = 1;
					intervals[i][1] = range;
				}
				else{
					intervals[i][0] = intervals[i-1][1] + 1;
					intervals[i][1] = intervals[i-1][1] + range;
				}
			}
// 			pr_intervals(intervals,tr_num);
		
		
			/* creating threads */
			pthread_t tids[tr_num];
			for (int i = 0; i < tr_num; i++){
				/* 
				* in questo caso è richiesta una condition variable, in quanto un una volta 
				* passato l'intervallo (&intervals[i]) su cui deve lavorare, la thread lo deve 
				* memorizzare. Tuttavia succede spesso che l'intervallo viene modificato 
				* (dalla successiva esecuzione di questo ciclo) prima che la thread venga creata
				* e che sia riuscita a salvare i dati necessari per poter procedere. 
				* quindi prima di eseguire il successivo ciclo aspettiamo che la thread salvi i dati.
				* questo meccanismo è stato implementato tramite una condition variable.
				*/
				if ( (err = pthread_mutex_lock(&mu)) != 0) err_exit(err,"mutex lock error: "); 
				if ( (err = pthread_create(&tids[i], NULL, calc_mantissa, &intervals[i] )) != 0 ) err_exit(err,"thread error: ");
				if ( (err = pthread_cond_wait(&cond, &mu)) != 0) err_exit(err,"cond waid error: "); 
				if ( (err = pthread_mutex_unlock(&mu)) != 0) err_exit(err,"mutex unlock error: ");
			}
			
			/* waiting for all threads */
			for (int i = 0; i < tr_num; i++){
				pthread_join(tids[i],NULL);
			}
			
			printf("\n\n");

			t2 = clock();  
			float diff = ((float)(t2 - t1) / 1000000.0F ) * 1000;   
			printf("REPORT");
			printf("thread number %d\n", tr_num);
			printf("range of number per thread %d\n", range);
			printf("time elapsed %f millis\n", diff);
			
		}
		tr_num++;
		
	}    
	                                                                                                                              
                                             
    return 0;                                                                                         
}



/*calc_mantissa  */
void* calc_mantissa(void *interval){

	/* 
	 * int *ptr punta al primo elemento dell'array passato come argomento, ptr++ causa
	 * uno shift del puntatore di 8 byte (le dimensioni di un int) e quindi punta al 
	 * secondo elemento dell'array.
	 * è molto importante il casting in quanto avendo dichiarato interval come void*, il
	 * compilatore non conosce le dimensioni di interval e di conseguenza ti riporta un 
	 * errore in quanto si sta un tipo di dimensioni sconosciute dentro un int che invece
	 * ha 8 byte
	 */
 	int *ptr = (int*)interval;
 	int err;
 	pthread_t tid;
 	tid = pthread_self();
 	char buf[6];
 	
 	
 	if ( (err = pthread_mutex_lock(&mu)) != 0) err_exit(err,"mutex lock error: ");
	int a = *ptr;
	ptr++;
	int b = *ptr;
// 	printf(" %d  %d\n",a,b);
	if ( (err = pthread_cond_signal(&cond)) != 0) err_exit(err,"cond signal error: ");
	if ( (err = pthread_mutex_unlock(&mu)) != 0) err_exit(err,"mutex unlock error: ");
	
	/* calcolo mantissa */ 
	for (int i = a; i <= b; i++){
		int n;
 		float(m);     
 		m = frexp(i, &n);	/* break floating-point number into normalized fraction(mantissa) and power of 2 */
 		
 		char *str = gcvt(m,6,buf);
 		//printf("esponente %d mantissa %s numero %d\n",n,buf,i);
		if( write(filedes[1],str,strlen(str)) != strlen(str))
			continue;

	}
	return 0;       
}



void pr_intervals(int intervals[][2], int rows){
	for(int i = 0; i < rows; i++){
		printf("interv[%d] : %d   %d", i, intervals[i][0], intervals[i][1]);
		printf("\n");
	}

}



/* non è usata da nessuna parte in questo programma */
int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len) { 

	struct flock lock;
	lock.l_type = type; 	/* F_RDLCK, F_WRLCK, F_UNLCK */ 
	lock.l_start = offset; 	/* byte offset, relative to l_whence */ 
	lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */ 
	lock.l_len = len; 		/* #bytes (0 means to EOF) */

	return(fcntl(fd, cmd, &lock));

}



void exit_handler(){
 	if ( kill(pid, SIGINT) == -1) err_sys("kill error\n");
}