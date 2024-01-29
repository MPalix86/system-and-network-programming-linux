/*
*	author : 		Mirco Palese
*	program name : 	exec
*	last edit : 	...
*	note :			print all passed argument and any environment variables
*/

#include <stdio.h>
#include <unistd.h>

int main (int argc , char *argv[] , char *envp[]){
	
	printf("ECHOALL\n");
	
	for (int i = 0 ; i < argc ; i++){
		if(argv[i] == (char *) 0 ) break;
		printf ("argv[%d] : %s\n", i, argv[i]);
		sleep(1);
	}
	
	printf("\n");
	
	for (int i = 0 ; ; i++){
		if(envp[i] == NULL) break;
		printf ("envp[%d] : %s\n", i, envp[i]);
		sleep(1);
	}
	
	printf("END\n\n");
	sleep(1);
	
	return 0;
	
}