#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

//#define PNUM 17500
#define PNUM 17500
#define MAXLINE 4096
#define PLENGTH 60

void err_sys(const char *fmt, ...);
void err_exit(int error, const char *fmt, ...);
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap);

int main(void)
{
    char packets[PNUM][PLENGTH];

    /* -----recovering mutex inside shared memory */
    pthread_mutex_t *mu;
    int shmem_mu_id;
    void *shmem_mu_ptr;
    int mu_err;

    /* getting shared m. id for data*/
    if ((shmem_mu_id =  shmget((key_t)1111, 0, 0666 | IPC_CREAT)) == -1)
        err_sys("shared memory error ");

    /* getting shared m. pointer for data */
    if ((shmem_mu_ptr = shmat(shmem_mu_id, 0, 0)) == (void *)-1)
        err_sys("shmat error");

    /* make mu, that expects a memory area made like pthread_mutex_t *, to point the shared memory. */
    mu = (pthread_mutex_t *)shmem_mu_ptr;

    printf("mutex recovered\n");

    /* -----recovering condition variable inside shared memory */
    void *shmem_cond_ptr;           /* pointer to cond shared memory */
    int shmem_cond_id;              /* shared memory cond id */
    pthread_cond_t *cond;

    /* getting shared m. id for cond*/
    if ((shmem_cond_id =  shmget((key_t)2222, 0, 0666 | IPC_CREAT)) == -1)
        err_sys("shared memory error ");

    /* getting shared m. pointer for cond */
    if ((shmem_cond_ptr = shmat(shmem_cond_id, 0, 0)) == (void *)-1)
        err_sys("shmat error");

    /* make cond, that expects a memory area made like pthread_cond_t , to point the shared memory. */
    cond = (pthread_cond_t *)shmem_cond_ptr;
    printf("condition recovered\n");

    /* recovering shared memory read with data */
    int shmem;
    char *shmem_ptr;

    if ((shmem = shmget(3333, 0, 0666)) == -1)
        err_sys("shared memory error ");
    if ((shmem_ptr = shmat(shmem, 0, 0)) == (void *)-1)
        err_sys("shmat error child");
    printf("data memory recovered\n");

    if ((mu_err = pthread_mutex_lock(mu)) != 0)
        err_exit(mu_err, "lock mutex error");

    /* try to read data inside shared memory */
    for (int i = 0; i < PNUM; i++)
    {
        strcpy(packets[i], shmem_ptr);
        printf("data read, iteration number: %d [%s]\n", i, packets[i]);
        if ((mu_err = pthread_cond_signal(cond)) != 0)
            err_exit(mu_err, "cond signal error: ");
        if ((mu_err = pthread_cond_wait(cond, mu)) != 0)
            err_exit(mu_err, "cond wait error");
    }

    if ((mu_err = pthread_mutex_unlock(mu)) != 0)
        err_exit(mu_err, "mutex unlock error");
    return 0;
}

/* from apue to print errors*/
void err_sys(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    exit(1);
}

void err_exit(int error, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, error, fmt, ap);
    va_end(ap);
    exit(1);
}

static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
    char buf[MAXLINE];

    vsnprintf(buf, MAXLINE - 1, fmt, ap);
    if (errnoflag)
        snprintf(buf + strlen(buf), MAXLINE - strlen(buf) - 1, ": %s",
                 strerror(error));
    strcat(buf, "\n");
    fflush(stdout);     /* in case stdout and stderr are the same */
    fputs(buf, stderr);
    fflush(NULL);       /* flushes all stdio output streams */
}