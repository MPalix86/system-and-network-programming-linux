/*
*	author : Mirco Palese
*	program name : lightbulb
*	last edit :	
*	description : 
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <fcntl.h>
#include "../../include/apue.h"


/* 
* SIGUSR1 handle red
* SIGUSR2 handle green
*/

void sig_handler(int, siginfo_t *str, void * val);
static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);


bool red_is_on = false;
bool green_is_on = false;
bool sig_r = false;
bool sig_g = false;
static int sem_id;
bool go_out = false;
int counter = 0;


int main (int argc , char *argv[]){
	
	struct sigaction sa; /* sigaction structure to define the signals handlers behavior */
	
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; /* Restart functions if interrupted by handler */
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) err_sys("sigaction error ");
    if (sigaction(SIGUSR2, &sa, NULL) == -1) err_sys("sigaction error ");
    if (sigaction(SIGINT, &sa, NULL) == -1) err_sys("sigaction error ");
	
	for(;;){
		if(!go_out){
			if(sig_r){
				//printf("\033[0m"); /* print default terminal color */
				if(red_is_on){
					 //printf("red is on\n");
					 counter++;
				}
				else //printf("red is off\n");
				sig_r = false;
			}
			else if(sig_g){
				//printf("\033[0m"); /* print default terminal color */
				if(green_is_on){
					//printf("green is on\n");
					counter++;
				} 
				else //printf("green is off\n");
				sig_g = false;
			}

		}
		else break;
	}
		
	
	printf("counter %d\n\n",counter);
	return 0;
	
}



void sig_handler(int sig, siginfo_t *siginfo, void * val){

		if( sig == SIGUSR1){
			
			red_is_on = !red_is_on;
			sig_r = true;
		}

		if( sig == SIGUSR2){
			green_is_on = !green_is_on;
			sig_g = true;
		} 
		
		
		if(sig == SIGINT){
			go_out = true;
		} 
}
