/*
*	author : Mirco Palese
*	program name : s29
*	last edit :	13/02/2020
*	description : see esameSNP29.txt
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include ""../../include/apue.h"
#include <arpa/inet.h>
#include <fcntl.h>
//-----------------------
#define BUFSIZE 4096	/* bufsize for receiving data */
#define CONN_NUMBER 5	/* number of request to the website */


pthread_mutex_t mu ;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

char *host;			/* host web site */
char *port = "80";	/* port 80 for web */
char *storage;		/* memory storage to save web page */
int  char_count;	/* counter of chars inside storage */

void *send_request(void *val);	/* send http request and receive result */
int create_socket_connect();	/* create socket */
void *save_request(void *val);	/* save storage inside file */

int main (int argc , char *argv[]){
	pthread_t tid1;
	int err, fd;

	printf("usage : specify website as parameter (i.e. www.somethingyouwant.com) \n\n");
	
	if(argc != 2 ){
		printf("ERROR: argument error\n");
		exit(0);
	}
	
	host = argv[1];
	
	/* open and close file only to truncate it if exists */
	if ( (fd = open("./response.txt", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) err_sys("file open error: ");
 	close(fd);
 	
 	for (int i = 0; i < CONN_NUMBER; i++){
 		pthread_mutex_init(&mu, NULL);	
		if ( (err = pthread_create(&tid1, NULL, send_request, (void *)0 )) != 0 ) 	
			err_exit(err,"thread error: ");
		pthread_join(tid1, NULL); 
		sleep (5);
	}

	return 0;
}




/* thread 1, send http request on specified host and wait for thread 2 */
void *send_request(void *fd){

	printf("thread 1 start\n");
	
	int sfd, err;
	char *http_req = "GET / HTTP/1.0\n\n";
	pthread_t tid2;
	char buf[BUFSIZE];
	int ret;
	
	char_count = 0;					/* initialize counter to 0 */
	storage = malloc(1);	/* allocating memory for storage. */ 		
										
	
	if ( (err = pthread_mutex_lock(&mu)) != 0) err_exit(err,"mutex lock error: "); /* lock mutex before creating thread 2 */
	if ( (err = pthread_create(&tid2, NULL, save_request, (void *)0 )) != 0 ) err_exit(err,"thread error: ");
 	sfd = create_socket_connect();
 	
	if( send(sfd, http_req, (size_t)strlen(http_req) + 1, (int) NULL) < 0) err_sys("http request error: ");
	for(;;){
		/* read received data */
		if ( (ret = recv(sfd, buf, BUFSIZE, NULL)) > 0){
			storage = realloc(storage, sizeof(storage) + ret); /* dinamically reallocates storage based on bytes read */
			strcat(storage, buf); 	/* append buf inside storage */
			char_count += ret;		/* save total amount of char to avoid write of undesired chars in the 2° thread */	
		}
		/* all data received and connection closed by host */
		else if (ret == 0) {
			printf("reception terminated. Connection closed by host\n");
			close(sfd);
			pthread_cond_wait(&cond, &mu); /*wait for 2 thread */
			printf("signal received\n");
			break;
		}
		/* some error occurred */
		else err_sys("recv error: ");
	}
	
	pthread_join(tid2, NULL);
	pthread_exit(NULL);
}



/* thread 2. Save response acquired by thread 1 and wiat for thread 1 */
void *save_request(void *val){
	
	printf("trhead 2 start\n");
	int fd;
	int err;
	char *separator = "\n\n\n\n";
	
	if ( (fd = open("./response.txt", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) err_sys("file open error: ");
	if( ( err = pthread_mutex_lock(&mu)) != 0) err_exit(err, "mutex unlock error");
	if( write(fd,storage,char_count) != char_count) err_sys("write error ");
	if( write(fd,separator,strlen(separator)) != strlen(separator)) err_sys("write error ");
	printf("response saved, sending signal to thread 1\n");
	if( (err = pthread_mutex_unlock(&mu)) !=0) err_exit(err,"mutex unlock error: ");
	pthread_cond_signal(&cond);
	close(fd);
	pthread_exit(0);
}






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


