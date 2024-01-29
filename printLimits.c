/*
*	author : 		Mirco Palese
*	program name : 	exec
*	last edit :		....
*	note : 			print limits defined on MAC-OS MOJAVE , type 'man getrlimit' for info
*/


#include <stdio.h>
#include <sys/resource.h>

#define doit(resource) printLimit(#resource, resource) /*chiedere ad emilio*/

static void printLimit(char *name, int resource);

int main (int argc, char *argv[]){
	
	/* Macro defined on MAC-OS Mojave */
	doit(RLIMIT_CORE);
	doit(RLIMIT_CPU);
	doit(RLIMIT_DATA);				
	doit(RLIMIT_FSIZE);
	doit(RLIMIT_MEMLOCK);
	doit(RLIMIT_NOFILE);
	doit(RLIMIT_NPROC);
	doit(RLIMIT_RSS);
	doit(RLIMIT_STACK );

	return 0;
}

static void printLimit(char *name, int resource){
	struct rlimit 	rl;
	struct rlimit 	*rl_ptr = &rl;

	if ( getrlimit(resource, rl_ptr) < 0 ) printf("ERROR\n");
	else {
		if (rl_ptr->rlim_cur == RLIM_INFINITY) printf("current limit of %s : infinite\n", name);
		else printf("current limit of %s : %lld\n", name, rl_ptr->rlim_cur);
		
		if (rl_ptr->rlim_max == RLIM_INFINITY) printf("maximum limit of %s : infinite\n", name);
		else printf("maximum limit of %s : %lld\n", name, resource);
		
		printf("value of %s : %d\n\n", name, resource);
	}
}


/* 	
	struct rlimit 	rl;
	struct rlimit 	*rl_ptr = &rl;
	if ( getrlimit(RLIMIT_CORE , rl_ptr) < 0 ) printf("ERROR\n");
	printf("current limit of %s : %lld\n", resource, rl_ptr->rlim_cur);
	
	is equivalent to:
	 
	struct rlimit rl
	if ( getrlimit(RLIMIT_CORE , &rl) < 0 ) printf("ERROR\n");
	printf("current limit of %s : %lld\n", resource, rl.rlim_cur);
*/






