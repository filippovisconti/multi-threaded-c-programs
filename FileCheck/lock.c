#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

pthread_spinlock_t ptspin;
pthread_mutex_t ptmutex;

void *foo(void *param)
{
    pthread_spin_lock(&ptspin);
    printf("Critical section one: spinlock \n");
    sleep(2);
    pthread_spin_unlock(&ptspin);

    pthread_mutex_lock(&ptmutex);
    printf("Critical section two: mutex \n");
    sleep(3);
    pthread_mutex_unlock(&ptmutex);
}

int main(int argc, char const *argv[])
{
    int nthreads = 8;
    pthread_t ptids[nthreads];

    pthread_spin_init(&ptspin, PTHREAD_PROCESS_PRIVATE);
    pthread_mutex_init(&ptmutex, PTHREAD_PROCESS_PRIVATE);

    for (int i = 0; i < nthreads; i++)
        pthread_create(ptids + i, NULL, foo, NULL);

    for (int i = 0; i < nthreads; i++)
        pthread_join(ptids[i], NULL);

    pthread_spin_destroy(&ptspin);
    pthread_mutex_destroy(&ptmutex);
}
