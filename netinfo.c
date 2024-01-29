/*
*	author : Mirco Palese
*	program name : network_indfo
*	last edit :	12/02/2020
*	description : recovering all network informations
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

void print_hostent();
void print_netent();

int main (int argc , char *argv[]){
	
	print_hostent();
 	print_netent();
	
	return 0;
	
}




/*
* Print hostent structure.
* This data type is used to represent an entry in the hosts database. 
*
* 		typedef struct hostent {
* 			char  *h_name;			/* the official name of the host (PC)
*  			char  **h_aliases;		/* NULL-terminated array of alternate name
*  			short h_addrtype;		/* The type of address being returned
*  			short h_length;			/* he length, in bytes, of each address
*  			char  **h_addr_list;	/* A NULL-terminated list of addresses for the host. 
*									   Addresses are returned in network byte order. 
*		}
*
*
*/
void print_hostent(){
	
	struct hostent *h;
	char addr_buf[INET_ADDRSTRLEN];  // INET_ADDRSTRLEN is a constant that specify the size of AF_INET address
	char **addr_list, **aliases_list;
	
	
	printf ("HOSTENT STRUCTURE\n");
	while ( (h = gethostent()) != NULL ){

		printf("h_name: %s\n", h->h_name);


		aliases_list = (char **)h->h_aliases;
		for(int i = 0; aliases_list[i] != NULL; i++) {
   			printf("h_aliases: %s\n", aliases_list[i]);
		}
		
		printf("h_addrtype: %d\n", h->h_addrtype);
		printf("h_lenght: %d bytes\n", h->h_length);
		
		inet_ntop(AF_INET, h->h_addr,addr_buf,INET_ADDRSTRLEN);			// converting address into string inside buf
		printf("h_addr: %s\n", addr_buf);
		
		/* 
		* for more info about why addr_list is declared as char ** and is read through inet_ntop
		* function (that instead read structures of type in_addr) read :
		* /Users/mircopalese/MEGAsync/Università/Magistrale/1 anno/Systems and network programming/my_programs/other/c networking structures oddities.txt
		*/
		addr_list = (char **)h->h_addr_list;
		for(int i = 0; addr_list[i] != NULL; i++) {
			inet_ntop(AF_INET, addr_list[i],addr_buf,INET_ADDRSTRLEN);		// converting address (struct in_addr) into string inside buf
   			printf("h_addr_list[%d]: %s\n",i, addr_buf);
		}
        
        
		printf("-----------------------------------\n\n");
	}
	
	endhostent(); /* close host file */
		
}



/*
* print netent structure.
* Structure for information from the network database
*
*     struct  netent {
*                   char            *n_name;        /* official name of net 
*                   char            **n_aliases;    /* alias list 
*                   int             n_addrtype;     /* net number type 
*                   uint32_t        n_net;          /* net number 
*           };
*
*/
void print_netent(){
	struct netent *net_str;
	char buf[INET_ADDRSTRLEN];  // INET_ADDRSTRLEN is a constant that specify the size of AF_INET address
	char **aliases;
	
	printf("NETENT STRUCTURE\n");
	while( (net_str = getnetent()) != NULL ){
 		printf("n_name: %s\n", net_str->n_name);
		
		aliases = net_str->n_aliases;
		for (int i = 0; aliases[i] != NULL ; i++){
			printf("n_aliases: %s\n", aliases[i]);
		}
		
		//inet_ntop(AF_INET, net_str.n_net, buf, INET_ADDRSTRLEN);		// converting address (struct in_addr) into string inside buffer
		
		printf("n_addrtype: %d\n", net_str->n_addrtype);
 		printf("n_net: %u\n", net_str->n_net);
		printf("-----------------------------------\n\n");
	}
	
	endnetent(); //closing file
}




/* other print_some_structure_function here */