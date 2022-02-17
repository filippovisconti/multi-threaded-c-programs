#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include "parameters.h"
// #include <sys/types.h>
// #include <sys/stat.h>

/*Si scriva il codice di una funzione C con la seguente interfaccia void tunnel(int descriptors[], int count) tale che, se eseguita, porti l’applicazione a gestire, per ogni file-descriptor dell’array descriptors l’inoltro del flusso dei dati in ingresso verso il canale di standard-output dell’applicazione. Il parametro count indica di quanti elementi è costituito l’array descriptors. L’inoltro dovrà essere attuato in modo concorrente per i diversi canali.*/
pthread_spinlock_t ptspin;

void *foo(void *param)
{
    int fd = *(int *)param;
    int size_r = 0;
    do
    {
        char buf[CHUNK];
        size_r = read(fd, buf, CHUNK);
        if (size_r < 0)
            abort("Read failed.\n");

        // it works even without locks, being STDOUT a continuous stream
        pthread_spin_lock(&ptspin);
        int size_w = write(STDOUT, buf, size_r);
        pthread_spin_unlock(&ptspin);
        if (size_w != size_r || size_w < 0)
            abort("write failed.\n");

    } while (size_r > 0);

    return NULL;
}

void tunnel(int descriptors[], int count)
{
    pthread_spin_init(&ptspin, PTHREAD_PROCESS_PRIVATE);
    pthread_t ptids[count];
    for (int i = 0; i < count; i++)
        pthread_create(ptids + i, NULL, foo, &descriptors[i]);

    for (int i = 0; i < count; i++)
        pthread_join(ptids[i], NULL);
    pthread_spin_destroy(&ptspin);
}

int main(int argc, char const *argv[])
{
    int count = COUNT;
    char *fnames[count];

    for (int i = 0; i < count; i++)
    {
        fnames[i] = malloc(11 * sizeof(char));
        fnames[i][0] = 'f';
        fnames[i][1] = 'i';
        fnames[i][2] = 'l';
        fnames[i][3] = 'e';
        fnames[i][4] = '_';
        fnames[i][5] = 'a' + i;
        fnames[i][6] = '.';
        fnames[i][7] = 't';
        fnames[i][8] = 'x';
        fnames[i][9] = 't';
        fnames[i][10] = '\0';
    }
    int *fildes = calloc(count, sizeof(int));

    for (int i = 0; i < count; i++)
        fildes[i] = open(fnames[i], O_RDONLY);

    printf("*Begin tunnelling to stout*\n");
    tunnel(fildes, count);
    printf("\n*Finished tunnelling to stout*\n");

    for (int i = 0; i < count; i++)
        close(fildes[i]);
}
