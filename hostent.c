/*
*	author : Mirco Palese
*	program name : hostent
*	last edit :	10/02/2019
*	description : hostent function call test
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#define h_addr  h_addr_list[0] 

int main (int argc , char *argv[]){
	
	struct hostent *h;
	char buf[INET_ADDRSTRLEN];  // INET_ADDRSTRLEN is a constant that specify the size of AF_INET address
	
	int i = 0;
	while ( (h = gethostent()) != NULL ){

		printf("h_name: %s\n", h->h_name);
		
		for (int j = 0; j < sizeof(h->h_aliases); j++){
			if(h->h_aliases[i] != NULL) printf("h_aliases: %s\n", h->h_aliases[i]);
		}
		
		printf("h_addrtype: %d\n", h->h_addrtype);
		printf("h_lenght: %d bytes\n", h->h_length);
		
		inet_ntop(AF_INET, h->h_addr,buf,INET_ADDRSTRLEN);			// converting address into string inside buf
		printf("h_addr: %s\n", buf);

       	for (int j = 0; j < sizeof(h->h_addr_list); j++){
       		inet_ntop(AF_INET, h->h_addr_list[i],buf,INET_ADDRSTRLEN);	// converting address into string inside buf
			printf("h_addr_list: %s\n",buf);
		}
        
        
		printf("-----------------------------------\n\n");
		i++;
		sleep(1);
		
	}
	
	
	
	exit(0);
	
}