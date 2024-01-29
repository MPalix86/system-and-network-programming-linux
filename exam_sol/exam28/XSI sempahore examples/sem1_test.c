/* After the #includes, the function prototypes and the global variable, we come to the
 main function. There the semaphore is created with a call to semget, which returns the
 semaphore ID. If the program is the first to be called (i.e. it's called with a parameter
 and argc > 1), a call is made to set_semvalue to initialize the semaphore and op_char is
 set to X. */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* #include "semun.h" */

static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);

static int sem_id;


int main(int argc, char *argv[])
{
    int i;
    int pause_time;
    char op_char = 'O';

    union semun sem_sample;
#ifdef _POSIX_SOURCE
   printf("_POSIX_SOURCE defined\n");
#endif

#ifdef _POSIX_C_SOURCE
   printf("_POSIX_C_SOURCE defined: %ldL\n", (long) _POSIX_C_SOURCE);
#endif

#ifdef _ISOC99_SOURCE
   printf("_ISOC99_SOURCE defined\n");
#endif

#ifdef _XOPEN_SOURCE
   printf("_XOPEN_SOURCE defined: %d\n", _XOPEN_SOURCE);

#endif

#ifdef _XOPEN_SOURCE_EXTENDED
   printf("_XOPEN_SOURCE_EXTENDED defined\n");
#endif

#ifdef _LARGEFILE64_SOURCE
   printf("_LARGEFILE64_SOURCE defined\n");
#endif

#ifdef _FILE_OFFSET_BITS
   printf("_FILE_OFFSET_BITS defined: %d\n", _FILE_OFFSET_BITS);
#endif

#ifdef _BSD_SOURCE
   printf("_BSD_SOURCE defined\n");
#endif

#ifdef _SVID_SOURCE
   printf("_SVID_SOURCE defined\n");
#endif

#ifdef _ATFILE_SOURCE
   printf("_ATFILE_SOURCE defined\n");
#endif

#ifdef _GNU_SOURCE
   printf("_GNU_SOURCE defined\n");
#endif

#ifdef _REENTRANT
   printf("_REENTRANT defined\n");
#endif

#ifdef _THREAD_SAFE
   printf("_THREAD_SAFE defined\n");
#endif

#ifdef _FORTIFY_SOURCE
   printf("_FORTIFY_SOURCE defined\n");
#endif

    srand((unsigned int)getpid());
    
    printf("sizeof(semum)=%lu\n", sizeof(sem_sample)); 
        
    sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);

    if (argc > 1) {
        if (!set_semvalue()) {
            fprintf(stderr, "Failed to initialize semaphore\n");
            exit(EXIT_FAILURE);
        }
        op_char = 'X';
        sleep(2);
    }

/* Then we have a loop which enters and leaves the critical section ten times.
 There, we first make a call to semaphore_p which sets the semaphore to wait, as
 this program is about to enter the critical section. */

    for(i = 0; i < 10; i++) {        

        if (!semaphore_p()) exit(EXIT_FAILURE);
        printf("%c", op_char);fflush(stdout);
        pause_time = rand() % 3;
        sleep(pause_time);
        printf("%c", op_char);fflush(stdout);

/* After the critical section, we call semaphore_v, setting the semaphore available,
 before going through the for loop again after a random wait. After the loop, the call
 to del_semvalue is made to clean up the code. */

        if (!semaphore_v()) exit(EXIT_FAILURE);
        
        pause_time = rand() % 2;
        sleep(pause_time);
    }    

    printf("\n%d - finished\n", getpid());

    if (argc > 1) {    
        sleep(10);
        del_semvalue();
    }
        
    exit(EXIT_SUCCESS);
}

/* The function set_semvalue initializes the semaphore using the SETVAL command in a
 semctl call. We need to do this before we can use the semaphore. */

static int set_semvalue(void)
{
    union semun sem_union;

    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
    return(1);
}

/* The del_semvalue function has almost the same form, except the call to semctl uses
 the command IPC_RMID to remove the semaphore's ID. */

static void del_semvalue(void)
{
    union semun sem_union;
    
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "Failed to delete semaphore\n");
}

/* semaphore_p changes the semaphore by -1 (waiting). */

static int semaphore_p(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
}

/* semaphore_v is similar except for setting the sem_op part of the sembuf structure to 1,
 so that the semaphore becomes available. */

static int semaphore_v(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
}

