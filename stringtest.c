/*
*	author : Mirco Palese
*	program name : 
*	last edit :	
*	description : 
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>

int main (int argc , char *argv[]){
	
	char *a = "porcoddio";
	char *b = "porcamadonna";
	char **pp;
	char **pp1[2];
	char **dpp;
	pp = &a;
	pp1[0] = &a;
	pp1[1] = &b;
	dpp = malloc(2 * sizeof(char*));
	dpp[0] = &a;
	dpp[1] = &b;
	
	
	printf("a[0]: %c\n", a[0]);			//value of first char inside string a
	printf("a: %s\n", a);				//entire string a (address of first character pointed by a)
	printf("*a: %c\n", *a);				//value inside address pointed by a (first character of string)
	printf("*a: %d\n", *a+2);			//value inside memory address pointed by a plus 2
	printf("&a[0]: %p\n", &a[0]);		//address of first character pointed by a (a = &a[0])
	printf("a: %p\n", a);				//address of first character pointed by a
	printf("&a: %p\n", &a);				//address of "variable" a
	printf("------------------------------------\n");
	printf("&pp: %p\n", &pp);			//adress of pp
	printf("&*pp: %p\n", &*pp);			//adress of "variable" a
	printf("&**pp: %p\n", &**pp);		//adress of value pointed by a
	printf("pp: %p\n", pp);				//adress pointed by pp (&a)
	printf("*pp: %p\n",*pp);			//address pointed by a (&a[0] = a) (%s instead of %p will cause the strig print)
	printf("**pp: %c\n",**pp);			//value of the address pointed by a (a[0] = *a)
 	printf("------------------------------------\n");
 	printf("pp1: %p\n", pp1);			//memory address of pp1
 	printf("&pp1: %p\n", &pp1);			//memory address of pp1
 	printf("&pp1[0]: %p\n", &pp1[0]);	//memory address of pp1
 	printf("&pp1[1]: %p\n", &pp1[1]);	//memory address of next element of pp1
 	printf(".....................\n");
 	printf("*pp1: %p\n", *pp1);			//address pointed by pp1[0] (or address of a)
 	printf("*pp1[0] %p\n", *pp1[0]);	//address pointed by a (or &a[0]) (%s instead of %p will cause the strig print)
 	printf("*pp1[1] %p\n", *pp1[1]);	//address pointed by b (or &b[0]) (%s instead of %p will cause the strig print)
 	printf("&*pp1: %p\n",&*pp1);		//address of pp1
 	printf("&*pp1[0]: %p\n",&*pp1[0]);	//address pointed by pp1[0] (or address of a)
 	printf("&*pp1[1]: %p\n",&*pp1[1]);	//address pointed by pp1[1] (or address of b)
 	printf(".....................\n");		
 	printf("&pp1: %p\n",&pp1);			//address pointed by a (&a[0] = a)
// 	printf("pp1[0]: %p\n",pp1[0]);		//address pointed by a (&a[0] = a)
// 	printf("&pp1[1]: %p\n",&pp1[1]);	//address pointed by a (&a[0] = a)
// 	printf("------------------------------------\n");
// 	printf("dpp[0]: %p\n",dpp[0]);		//address pointed by a (&a[0] = a)
//	printf("dpp[1]: %p\n",dpp[1]);		//address pointed by a (&a[0] = a)
	
	
	return 0;
	
}