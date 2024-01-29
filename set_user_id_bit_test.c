/*
*	author : 		Mirco Palese
*	program name : 		set_user_id_bit_test
*	last edit :		16/01/2020
*	note :			this file and relative binary executable is owned by root with the set_user_id_bit enabled.
*/


#include <stdio.h>
#include <stdlib.h>
#include <apue.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	uid_t real_uid;
	uid_t effective_uid;
	int res;	
	
	real_uid = getuid();
	effective_uid = geteuid();

	printf("realt user id: %d\n",real_uid);
	printf("effective user id %d\n",effective_uid);

	if( (res = setreuid(0,0)) == 0){
                real_uid = getuid();
                effective_uid = geteuid();
                printf("done setreuid(0,0\n");
                printf("real user id: %d\n",real_uid);
                printf("effective user id: %d\n",effective_uid);
        }
	else if (res == -1){
		printf("error occurred during setreuid(0,0)\n");
	}	
	
	return 0;
}
