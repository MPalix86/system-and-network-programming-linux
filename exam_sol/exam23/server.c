/*
*	author : Mirco Palese
*	program name : sock_server
*	last edit :	13/02/2020
*	description : basic socket server
*	note :
*
*	The bind() function accepts a pointer to a sockaddr, but in all examples I've seen, a sockaddr_in structure is used instead, and is cast to sockaddr:
*	struct sockaddr_in name;
*	
*	for example:
*	if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
*	
*	I can't wrap my head around why is a sockaddr_in struct used. Why not just prepare and pass a sockaddr?
*
*	Is it just convention?
*
*	No, it's not just convention. 	
*	sockaddr is a generic descriptor for any kind of socket operation, whereas sockaddr_in 
*	is a struct specific to IP-based communication (IIRC, "in" stands for "InterNet"). 
*	As far as I know, this is a kind of "polymorphism" : the bind() function pretends to take 
*	a struct sockaddr *, but in fact, it will assume that the appropriate type of structure 
*	is passed in; i. e. one that corresponds to the type of socket you give it as the first argument.
*
*
*		on both bind() and getsockname
*       The actual structure passed for the addr argument will depend on the
*       address family.  The sockaddr structure is defined as something like:
*
*           struct sockaddr {
*               sa_family_t sa_family;
*               char        sa_data[14];
*           }
*
*       The only purpose of this structure is to cast the structure pointer
*       passed in addr in order to avoid compiler warnings. 
*
*		for AF_INET use:
*
*		 struct sockaddr_in {
*               sa_family_t    sin_family; // address family: AF_INET 
*               in_port_t      sin_port;   // port in network byte order 
*               struct in_addr sin_addr;   // internet address 
*           };
*
*           // Internet address. 
*           struct in_addr {
*               uint32_t       s_addr;     // address in network byte order 
*           };
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "../../include/apue.h"
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>  
#include <inttypes.h>
#define MAX_CONN 5
#define BUFFSIZE 1024


void* create_socket(void *val);
void printv(int *v, int v_size);


int *vector;
int v_size = 0;
pthread_mutex_t mu;
	
int main (int argc , char *argv[]){
	
	int err;
	pthread_t tid1 = NULL;
	pthread_t tid2 = NULL;
	pthread_t tid3 = NULL;
	
	vector = malloc(sizeof(int));
	vector[0] = 19;
	v_size++;
	
	pthread_mutex_init(&mu, NULL);
	
	/* creating thread */
	if ( (err = pthread_create(&tid1, NULL, create_socket, (void *)0 )) != 0 ) err_exit(err,"thread error: ");
	if ( (err = pthread_create(&tid2, NULL, create_socket, (void *)0 )) != 0 ) err_exit(err,"thread error: ");
	if ( (err = pthread_create(&tid3, NULL, create_socket, (void *)0 )) != 0 ) err_exit(err,"thread error: ");
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);

	
	return 0;
	
}










void* create_socket(void *val){
	int sfd, conn_sfd, num;
	struct sockaddr_in s_addr, s_addr_conn, s_addr_r;
	socklen_t socklen = sizeof(s_addr);
	char buf[INET_ADDRSTRLEN], read_buf[BUFFSIZE];
	int n;
	
	
	/* socket creation */
	sfd = socket(PF_INET,SOCK_STREAM,0);
	if( sfd < 0) err_sys("socket error\n");
	
	/* clean structure */
	memset (&s_addr, 0, sizeof(s_addr));
	
	/* 
	* assignment of address family (AF_INET represent ipv4 internet) to s_addr structure for binding.
	* The other filed of the structure if are void will be filled automatically by bind(),
	* but you can also specify them.
	*
	* note that if you don't specify the address, bind will assign 0.0.0.0.
	* 0.0.0.0 has a couple of different meanings, but in this context, when a server is told 
	* to listen on 0.0.0.0 that means "listen on every available network interface". The 
	* loopback adapter with IP address 127.0.0.1 from the perspective of the server process
	* looks just like any other network adapter on the machine, so a server told to listen 
	* on 0.0.0.0 will accept connections on that interface too.
	*/
	
	s_addr.sin_family = AF_INET;
	//s_addr.sin_port = 4000;
	inet_pton(AF_INET, "192.168.1.149", &s_addr.sin_addr); /* note that if ip address not belongs to the machine, bind will return error */
	
	/* bind socket 0*/
	if(bind(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0) err_sys("bind error: ");
	
	/* inet_ntop convert the struct "in_addr" into char inside buf to recove the ip address */
	inet_ntop(AF_INET, &s_addr.sin_addr, &buf, INET_ADDRSTRLEN);
	
	if( listen(sfd, MAX_CONN) < 0 ) err_sys("listen errror: ");
	printf("ip address: %s\n", buf);
	
	/* recovering bind informations */
	if(getsockname(sfd, (struct sockaddr *) &s_addr_r, &socklen) < 0) err_sys("getsockname error: ");
	
	/* 
	* If your pc processor it’s an Intel 80x86, Host Byte Order is Little-Endian (reverse order).
	* The standard on internet is Big-Endian, so when the sin_port is filled inside memory
	* getsockname fill it in Network bite order(Big-Endian) for compatibility reasons. Hence
	* you have to use a function like ntohs (Network-TO-HoSt (byte order))
	*/
	printf("port: %hu\n", ntohs(s_addr_r.sin_port));
	
	/* wait for connection and then accept it for all sockets*/
	conn_sfd = accept(sfd, &s_addr_conn, &socklen);
	
	if(conn_sfd < 0) err_sys("accept error: ");
	
	printf("connection accepted. Hit ^C to exit\n");
	printf("waiting for data...\n\n");
	
	/* reading loop from connected socket */
	for(;;){
		while ( (n = read(conn_sfd, read_buf, BUFFSIZE)) > 0){
			char tempbuf[n+1];
			memcpy(tempbuf,read_buf,n);
			tempbuf[n+1] = '\0';
			if( (num = strtol(tempbuf, NULL, 10)) == 0)
				err_sys("strtol error ");
			else{
				pthread_mutex_lock(&mu);
				vector = (int*) realloc(vector,sizeof(vector) + sizeof(int));
				vector[v_size] = num;
				v_size++;
				pthread_mutex_unlock(&mu);
				printv(vector,v_size);
				
			}
			
		}
	}
	
}




void printv(int *v, int v_size){
	for (int i = 0; i < v_size; i++){
		printf("%d ",v[i]);
	}
	printf("\n");
}