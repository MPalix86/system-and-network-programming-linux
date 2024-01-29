
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include "../../include/apue.h"


//#define PNUM 17500
#define PNUM 10
#define PLENGTH 60
#define MAXLINE 4096

int main(int argc, char **argv)
{
    /* creating new mutex inside shared memory area */
    void *shmem_mu_ptr;             /* pointer to mutex shared memory */
    pthread_mutex_t *mu;            /* pointer to mutex */
    int shmem_mu_id;                /* shared memory mutex id */
    int mu_err;
    size_t size = sizeof(pthread_mutex_t);  /* shared m. mutex size (size was already declared) */
    char packet[PLENGTH];

    for (int i = 0; i < PLENGTH; i++)
        packet[i] = 'a';
    packet[PLENGTH - 1] = '\0';

    /* getting shared m. id for mutex*/
    if ((shmem_mu_id =  shmget((key_t)1111, size, 0666 | IPC_CREAT)) == -1)
        err_sys("shared memory error ");

    /* getting shared m. pointer for mutex */
    if ((shmem_mu_ptr = shmat(shmem_mu_id, 0, 0)) == (void *)-1)
        err_sys("shmat error: ");

    /* make mu, that expects a memory area made like pthread_mutex_t , to point the shared memory. */
    mu = (pthread_mutex_t *)shmem_mu_ptr;

    /* initialize the attribute to prepare mutex to be placed in shared memory */
    int rc;
    pthread_mutexattr_t attr;
    if ((rc = pthread_mutexattr_init(&attr)) != 0)
        err_exit(rc, "pthread_mutexattr_init: ");
    if ((rc = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)) != 0)
        err_exit(rc, "pthread_mutexattr_setpshared: ");

    /* initialize mutex */
    if ((rc = pthread_mutex_init(mu, &attr)) != 0)
        err_exit(rc, "pthread_mutex_init: ");
    if ((rc = pthread_mutexattr_destroy(&attr)) != 0)
        err_exit(rc, "pthread_mutexattr_destroy: ");

    /* creating new condition variable inside shared memory area */
    void *shmem_cond_ptr;           /* pointer to cond shared memory */
    int shmem_cond_id;              /* shared memory cond id */
    pthread_cond_t *cond;
    size = sizeof(pthread_cond_t);  /* shared m. cond size (size was already declared) */

    /* getting shared m. id for cond*/
    if ((shmem_cond_id =  shmget((key_t)2222, size, 0666 | IPC_CREAT)) == -1)
        err_sys("shared memory error ");

    /* getting shared m. pointer for cond */
    if ((shmem_cond_ptr = shmat(shmem_cond_id, 0, 0)) == (void *)-1)
        err_sys("shmat error: ");

    /* make cond, that expects a memory area made like pthread_cond_t , to point the shared memory. */
    cond = (pthread_cond_t *)shmem_cond_ptr;

    /* * initialize the attribute to prepare cond to be placed in shared memory */
    pthread_condattr_t attrcond;
    if ((rc = pthread_condattr_init(&attrcond)) != 0)
       err_exit(rc, "pthread_condattr_init: ");
    if ((rc = pthread_condattr_setpshared(&attrcond, PTHREAD_PROCESS_SHARED)) != 0)
        err_exit(rc, "pthread_condattr_setpshared: ");

    /* Initialize condition. */
    if ((rc = pthread_cond_init(cond, &attrcond)) != 0)
        err_exit(rc, "pthread_cond_init: ");
    if ((rc = pthread_condattr_destroy(&attrcond)) != 0)
        err_exit(rc, "pthread_condattr_destroy: ");

    /* Clean up. */
    //pthread_condattr_destroy(&attrcond);

    /* creating new shared memory area to share only PLENGTH (60) byte at time */
    void *shmem_ptr60;
    int shmem60;
    char *shmem_char;
    size = sizeof(char) * PLENGTH;   /* shared m. data size (size was already declared) */

    /* getting shared m. id */
    if ((shmem60 = shmget(3333, size, 0666 | IPC_CREAT)) == -1)
        err_sys("shared memory error ");

    /* getting shared m. pointer */
    if ((shmem_ptr60 = shmat(shmem60, 0, 0)) == (void *)-1)
        err_sys("shmat error: ");

    shmem_char = shmem_ptr60;

    /* locking mutex before creating new process to avoid child reading void memory area */
    if ((mu_err = pthread_mutex_lock(mu)) != 0)
        err_exit(mu_err, "lock mutex error: ");

    /* calling child */
    pid_t pid;
    //if ((pid = vfork()) == -1)
    if ((pid = fork()) == -1)
        err_sys("fork error: ");
    else if (pid == 0)
    {
        if (execl("./test2", "test2", (void *)0) == -1)
            err_sys("exec error: ");
        exit(0);
    }
    //else if (pid == -1)
        //err_syserr("fork error: ");
    printf("Child forked OK\n");

    /*  copying data inside shared memory */
    for (int i = 0; i < PNUM; i++)
    {
        packet[i % (PLENGTH - 1)]++;
        strcpy(shmem_char, packet);
        printf("data written, iteration number: %d [%s]\n", i, packet);
        if ((mu_err = pthread_cond_signal(cond)) != 0)
            err_exit(mu_err, "pthread_cond_signal: ");
        if ((mu_err = pthread_cond_wait(cond, mu)) != 0)
            err_exit(mu_err, "pthread_cond_wait: ");
    }
    if ((mu_err = pthread_cond_signal(cond)) != 0)
        err_exit(mu_err, "pthread_cond_signal: ");
    if ((mu_err = pthread_mutex_unlock(mu)) != 0)
        err_exit(mu_err, "pthread_mutex_unlock: ");

    waitpid(pid, NULL, 0);

    if (shmctl(shmem_mu_id, IPC_RMID, 0) < 0)
        err_sys("shmctl error: ");
    if (shmctl(shmem_cond_id, IPC_RMID, 0) < 0)
        err_sys("shmctl error: ");
    if (shmctl(shmem60, IPC_RMID, 0) < 0)
        err_sys("shmctl error: ");

    return 0;
}