/*
*	author : Mirco Palese
*	program name : smtp_client
*	last edit :	26/02/2020
*	description : see esameSNP22.txt
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../../include/apue.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <ctype.h>
#include <stdbool.h>
 
//-----------------------
#define BUFSIZE 4096	/* bufsize for receiving data */
#define CONN_NUMBER 5	/* number of request to the server */

char *host;				/* host web site */
char *port = "587";		/* port 587 for SMTP */
pthread_t tid;
int fd;

void send_request(int sfd);		/* send http request and receive result */
int create_socket_connect();	/* create socket */
void* sig_thread(void *val);	/* thread that handle signals */
void sig_handler(int signo);	/* signals handler */
char* get_signame(int signo);	/* return signal name from signal number */
void upcase(char *s);			


int main (int argc , char *argv[]){

	int interval = atoi(argv[2]);
	host = argv[1];
	int err, unslept;
	struct sigaction act;
	struct timeval tm1,tm2;
	char str[64];
	
	printf("\033[0;33m");		/* yellow */
	printf("usage : specify website as parameter (i.e. www.somethingyouwant.com) and sleep time (i.e. 5)\n");
	printf("note that this client connect to the server on port 587 and not 25\nyou will see the result in log.txt\n\n");
	printf("\033[0m");			/* default color */
 	
	if(argc != 3 ){
		printf("ERROR: argument error\n");
		exit(0);
	}
	
	/* preparing struct for sigaction */
	act.sa_handler = sig_handler; 
	sigfillset(&act.sa_mask); 
	act.sa_flags = 0;
	
	#ifndef NSIG		/* NSIG define the signals number */
	int NSIG = 31;
	#endif
	
	/* signal number goes from 1 to NSIG */
	for (int i = 1; i <= NSIG; i++)
		sigaction(i,&act,NULL);
	
	
	if ( (err = pthread_create(&tid, NULL, sig_thread, (void*)0 )) != 0 ) err_exit(err,"thread error: ");
	
	/* open file */
	if ( (fd = open("./log.txt", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) err_sys("file open error: ");
 	
 	
 	for (int i = 0; i < CONN_NUMBER; i++){	
 		
 		gettimeofday(&tm1, NULL);				/* getting time */
 		int sfd = create_socket_connect();		/* creating socket and connect to the server */
 		send_request(sfd);						/* sending smtp request */
 		gettimeofday(&tm2, NULL);				/* getting time */
 		
 		/* calculating time to connect */
		unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
		
		/* putting calculated time inside string */
    	sprintf(str,"%llu", t);	
    	strcat(str, " millisecond\n");
    	
		printf("reception terminated. Connection closed by host\n");
		
		/* writing in a log file */
		if( write(fd,str,strlen(str)) != strlen(str)) err_sys("write error ");
		
		unslept = sleep (interval); /* 
									* remember that sleep function returns when signal is delivered.
		 				  			* If the sleep() function returns because the requested time has elapsed, the value returned will be zero.  If the sleep()
    					  			* function returns due to the delivery of a signal, the value returned will be the unslept amount (the requested time minus
     					 			* the time actually slept) in seconds.
     					 			*/
     	while(unslept != 0)				
     	 	unslept = sleep(unslept);
	}
	return 0;
}




/* send request on specified host */
void send_request(int sfd){
	char *smtp_req = "QUIT\n"; 	
	int ret;
	
 	printf("sending request \n");
	if( send(sfd, smtp_req, (size_t)strlen(smtp_req), (int) NULL) < 0) err_sys("smtp request error: ");
	for(;;){
		/* read received data */
		if ( (ret = recv(sfd, NULL, NULL, (void*)0)) > 0) continue;
		
		/* all data received and connection closed by host */
		else if (ret == 0) {
			close(sfd);
			break;
		}
		/* some error occurred */
		else err_sys("recv error: ");
	}
}




/* create socket and connect on specified server and specified port */
int create_socket_connect(){

	int sfd;
	struct addrinfo hints, *res;
	
	/* All unsetted elements of the addrinfo structure passed via hints must be zero or the null pointer. */
	memset(&hints, 0, sizeof(hints));
	
	hints.ai_family = PF_UNSPEC;		/* accept every type of connection */
	hints.ai_socktype = SOCK_STREAM;	
	
	/* use of getaddrinfo() to translate domain name */
	if( getaddrinfo(host, port, &hints, &res) != 0 ){
		err_sys("getaddrinfo error :");
	}
	if(res == NULL) err_quit("Can't connect to specify host\n");
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) err_sys("socket error: ");
	if(connect(sfd, res->ai_addr, res->ai_addrlen) < 0) err_sys("connect error: ");
	printf("connected to host: %s\n", host);
	return sfd;
}




/* thread that wait for signals and register the arrive in a log file */
void* sig_thread(void *val){
	
	char *format = "%d-%m-%Y- %H:%M:%S";
	struct timeval time;	/* contains time in milliseconds and nanoseconds */
	struct tm *f_time;		/* contains formatted time */
	char str[100];
	time_t nowtime;
	sigset_t set;
	int signo;
	
	for(;;){
		sigfillset(&set);							/* specifies the signal sets to wait for */
		sigwait(&set, &signo);						/* waiting for signals */		
		gettimeofday(&time,NULL);					/* getting time */
		nowtime = time.tv_sec;						/* putting %lu milliseconds into size_t nowtime for formatting data */
		f_time = localtime(&nowtime);				/* formatting data */
		strftime(str, sizeof(str),format,f_time);	/* putting formatted data inside string (res) */
		char* signame = get_signame(signo);			/* getting sig name */
		strcat(str, " ");							/* adding space between date and signame */
		strcat(signame, "\n");						/* adding newline after signame */
		strcat(str, signame);						/* concatenating str and signame */
		
		/* writing log */
		if( write(fd,str,strlen(str)) != strlen(str)) err_sys("write error ");
	}

}




/* signal handler */
void sig_handler(int signo){
	pthread_kill(tid,signo);	/* sending signal to thread */
}




/* get signal name */
char* get_signame(int signo){
        char *str = strdup(sys_signame[signo]);
        if (!str) return NULL;
        upcase(str);
    return str;
}




void upcase(char *s){
    while (*s){
        *s = toupper(*s);
        s++;        
    }
}