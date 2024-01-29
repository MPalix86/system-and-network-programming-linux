/*
*	author : Mirco Palese
*	program name : sock clinet
*	last edit :	13/02/2020
*	description : simple socket tcp client
*	note : 
* 	
*	hints (used in getaddrinfo) is an optional pointer to a struct addrinfo, as defined by <netdb.h>:
*
*     struct addrinfo {
*             int ai_flags;           		// input flags 
*             int ai_family;          		// protocol family for socket 
*             int ai_socktype;        		// socket type 
*             int ai_protocol;        		// protocol for socket 
*             socklen_t ai_addrlen;    	 	// length of socket-address 
*             struct sockaddr *ai_addr; 	// socket-address for socket 
*             char *ai_canonname;     		// canonical name for service location 
*             struct addrinfo *ai_next; 	// pointer to next in list 
*     };
*
*     This structure can be used to provide hints concerning the type of socket that the caller supports or wishes to use.  The
*     caller can supply the following structure elements in hints:
* 
*	 
*	note that in this case binding is not strictly necessary. Indeed, if during connect 
*	socket was not bound, kernel automatically assign the address
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "include/apue.h"
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define BUFFSIZE 4096



int main (int argc , char *argv[]){
	
	int sfd, n;
	struct addrinfo hints, *res, *p;
	char buf[BUFFSIZE];
	
	
	if(argc != 3 ){
		printf("ERROR: argument error\n");
		printf("usage : server_addr port_num\n");
		exit(0);
	}
	
	char *host = argv[1];
	char *port = argv[2];
	
	/* All unsetted elements of the addrinfo structure passed via hints must be zero or the null pointer. */
	memset(&hints, 0, sizeof(hints));
	
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if( getaddrinfo(host, port, &hints, &res) != 0 ){
		err_sys("getaddrinfo error :");
	}
	
	// loop through all the results and connect to the first we can
	for(p = res; p != NULL; p = p->ai_next) {
		if ((sfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			continue;
		}

		if (connect(sfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sfd);
			continue;
		}
		break;
	}
	
	if(p == NULL) err_sys("connection error");
	printf("connected. Hit ^C to exit\n");
	printf("write somethings and press enter to send data...\n\n");
	
	for(;;){
		while ( (n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0){
			if( write(sfd,buf,n) != n){
				break;
			}
		}
	}
	
	
	return 0;
	
}