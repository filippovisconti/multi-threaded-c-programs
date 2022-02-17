#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "parameters.h"

#define THREAD_NUMBER FILE_SIZE / MEM_PER_CORE
#define CHUNK FILE_SIZE / (FILE_SIZE / MEM_PER_CORE)

volatile short min = 0x7fff, max = 0xffff;
// int fd;
short minmaxarray[THREAD_NUMBER * 2];
void *shared_seg;

typedef struct arguments
{
    int tid;
} args;

void *minmax(void *param)
{
    args *parametri = param;

    int fd = open(FILENAME, O_RDONLY);
    // printf("thread %d fd %d\n", parametri->tid, fd);
    if (fd == -1)
        abort("input open error\n");

    off_t offset = lseek(fd, CHUNK * parametri->tid, SEEK_SET);
    if (offset == -1)
        abort("offset error\n");

    short array[CHUNK];

    int size_r = read(fd, (void *)array, CHUNK * (int)sizeof(short));

    if (size_r == -1)
    {
        printf("Something went wrong with read()! %s\n", strerror(errno));
        abort("Abort. Read Failed.\n");
    }
    close(fd);
    // res0= min, res1=max
    short min = array[0];
    short max = array[0];
    // printf("nbytes %d, read %d, val %hi \n\n", sizeof(short), size_r, *array);
    for (int i = 0; i < CHUNK; i++)
    {
        if (array[i] < min)
            min = array[i];
        else if (array[i] > max)
            max = array[i];
    }
    int t = parametri->tid;
    minmaxarray[t * 2] = min;
    minmaxarray[t * 2 + 1] = max;
    return NULL;
}

/*
void *function(void *param)
{
    short local_data;
    short lmin = 0x7fff, lmax = 0xffff;
    short old_min, old_max;
    int count = 0;
    while (read(fd, &local_data, sizeof(short)) == sizeof(short))
    {
        if (lmin > local_data)
            lmin = local_data;
        if (lmax < local_data)
            lmax = local_data;
        count++;
    }
}
*/
/*
usare compare xcgh per verificare che il minimo letto sia rimasto uguale e non sia stato modificato in concorrenza da altri threads.
*/

int main(int argc, char const *argv[])
{
    pthread_t ptids[THREAD_NUMBER];
    args *argarray[THREAD_NUMBER];

    int j;

    for (j = 0; j < THREAD_NUMBER; j++)
    {
        argarray[j] = (args *)malloc(sizeof(args));
        argarray[j]->tid = j;
        pthread_create(ptids + j, NULL, minmax, (void *)argarray[j]);
        // printf("TO %d - ", j);
    }

    // printf("\n\n");
    //__sync_bool_compare_and_swap(&stop, 0, 1);

    for (j = 0; j < THREAD_NUMBER; j++)
    {
        pthread_join(ptids[j], NULL);
        // printf("TC %d - ", j);
        free(argarray[j]);
    }

    short min = minmaxarray[0];
    short max = minmaxarray[1];

    for (int i = 0; i < THREAD_NUMBER * 2; i++)
    {
        // printf("%d ", minmaxarray[i]);
        if (minmaxarray[i] < min)
            min = minmaxarray[i];
        else if (minmaxarray[i] > max)
            max = minmaxarray[i];
    }

    printf("\nMin: %d | -- | Max: %d\n", min, max);
    return 0;
}
