/*
*	author : 		Mirco Palese
*	program name : 	my_getc_putc
*	last edit :		9/01/2020
*/

#include "include/apue.h"

int
main(void)
{
	int		c;

	while ((c = getc(stdin)) != EOF)
		if (putc(c, stdout) != EOF) {
			fflush(stdout) ; 
			sleep(1);
		}
		else err_sys("output error");
		

	if (ferror(stdin))
		err_sys("input error");

	exit(0);
}