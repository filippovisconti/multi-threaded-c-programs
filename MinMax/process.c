#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "parameters.h"

#define CHUNK MEM_PER_CORE

volatile short min = 0x7fff, max = 0xffff;
int fd = -1;
short minmaxarray[CORES * 2];
void *shared_seg;

void *minmax(void *param)
{
    int tid = (int)param;

    short array[CHUNK];
    int size_r = 0;
    short oldmin = 0x7fff;
    short oldmax = 0xffff;

    do
    {
        size_r = read(fd, (void *)array, CHUNK * (int)sizeof(short));
        if (size_r == -1)
        {
            printf("Something went wrong with read()! %s\n", strerror(errno));
            abort("Abort. Read Failed.\n");
        }

        short min = array[0];
        short max = array[0];

        for (int i = 0; i < CHUNK; i++)
        {
            if (array[i] < min)
                min = array[i];
            else if (array[i] > max)
                max = array[i];
        }

        if (oldmin > min)
            oldmin = min;
        if (oldmax < max)
            oldmax = max;
    } while (size_r);

    minmaxarray[tid * 2] = oldmin;
    minmaxarray[tid * 2 + 1] = oldmax;
    return NULL;
}

int main(int argc, char const *argv[])
{
    fd = open(FILENAME, O_RDONLY);
    if (fd == -1)
        abort("input open error\n");

    pthread_t ptids[CORES];

    for (int j = 0; j < CORES; j++)
        pthread_create(ptids + j, NULL, minmax, (void *)j);

    for (int j = 0; j < CORES; j++)
        pthread_join(ptids[j], NULL);

    short min = minmaxarray[0];
    short max = minmaxarray[1];

    for (int i = 0; i < CORES * 2; i++)
    {
        if (minmaxarray[i] < min)
            min = minmaxarray[i];
        else if (minmaxarray[i] > max)
            max = minmaxarray[i];
    }

    printf("\nMin: %d | -- | Max: %d\n", min, max);
    return 0;
}
