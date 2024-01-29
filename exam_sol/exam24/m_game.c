/*
*	author : Mirco Palese
*	program name : 
*	last edit :	
*	description : 
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>

static struct termios oldSettings;

int move_cursor(int *x, int *y);

void kbcleanup( void )
{
    tcsetattr( 0, TCSAFLUSH, &oldSettings );     /* restore old settings */
}

void kbsetup( void )
{
    tcgetattr( 0, &oldSettings );

    struct termios newSettings = oldSettings;

    newSettings.c_lflag &= ~ICANON;   /* disable line-at-a-time input */
    newSettings.c_lflag &= ~ECHO;     /* disable echo */
    newSettings.c_cc[VMIN]  = 0;      /* don't wait for characters */
    newSettings.c_cc[VTIME] = 0;      /* no minimum wait time */

    if ( tcsetattr( 0, TCSAFLUSH, &newSettings ) == 0 ){
        atexit( kbcleanup );    /* restore the terminal settings when the program exits */
    } else {
        fprintf( stderr, "Unable to set terminal mode\n" );
        exit( 1 );
    }
}

int getkey( void )
{
    char c;

    if ( read( STDIN_FILENO, &c, 1 ) == 0 )
        return '\0';
    else
        return c;
}

int main( void ){
    int c;
    bool moved;
    
	int c_x = 0; /* player position x */
	int c_y = 0; /* player position y */

    kbsetup();

    time_t start = time( NULL );
    time_t previous = start;
    
    for (;;){
      	usleep( 1000 );
        time_t current = time( NULL );
        moved = false;
        fflush(stdin);
		if ( (c = getkey()) != '\0' ){
			printf("%c",c);
			if ( c == 'q' || c == 'Q' )
				exit(0);
			if (c == 'w' || c == 'a' || c == 's' || c == 'd')
				moved = true;
			if ( c == 'w'){
				if(c_y > 0) c_y--;
			}
			else if ( c == 'a'){
				if(c_x > 0) c_x--;
			}
			else if ( c == 's'){
				if(c_y < 20) c_y++;
			}
			else if ( c == 'd'){
				if(c_x < 20) c_x++;
			}
		}

		if(moved){
			printf("\e[1;1H\e[2J"); /* clear console screen */ 
			for (int y = 0;y <= 20; y++){
				for(int x = 0; x <= 20; x++){
					if (x == c_x && c_y == y)
						printf("\033[0;31mO\033[0m|");
					else
						printf("_|");
		
				}
				printf("\n");
			}   
		}
		
    }
}




int move_cursor(int *x, int *y){
	int c;
	printf("aspetto");

	return 0;
}