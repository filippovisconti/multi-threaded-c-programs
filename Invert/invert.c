#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "parameters.h"

int chunk = 0;
int th_num = 0;
char *buffer;
const char *filename;

int getfilesize(const char *fname)
{
    struct stat info;
    stat(fname, &info);
    return info.st_size;
}

int getfilesize2(const char *fname)
{
    int ifd = open(fname, O_RDONLY);
    if (ifd == -1)
        abort("input open error\n");
    int res = lseek(ifd, 0, SEEK_END);
    close(ifd);
    return res;
}

void *invert(void *param)
{
    // int size = getfilesize(ifd);
    int tid = (int)param;

    /* open the input file and check errors */
    int ifd = open(filename, O_RDONLY);
    if (ifd == -1)
        abort("input open error\n");

    // offset file pointer
    lseek(ifd, chunk * tid, SEEK_SET);

    // read the chunk from offset
    char temp[chunk];
    int size_r = read(ifd, temp, chunk);
    if (size_r == -1)
        abort("read failed.\n");

    // invert the chunk in temp array
    for (int i = 0; i < chunk / 2; i++)
    {
        char t = temp[i];
        temp[i] = temp[(chunk - 1) - i];
        temp[(chunk - 1) - i] = t;
    }

    // copy the inverted array into the final buffer
    for (int i = 0; i < chunk; i++)
    {
        buffer[chunk * (th_num - tid - 1) + i] = temp[i];
    }

    close(ifd);
    return NULL;
}

void fixRemainingBytes(int diff, int filesize)
{
    // open file to read the last unread bytes if any
    int ifd = open(filename, O_RDONLY);
    lseek(ifd, chunk * th_num, SEEK_SET);

    // allocate temporary array and fill it with values
    char *temp = malloc(diff);
    int size_r = read(ifd, temp, diff);
    if (size_r < 0 || size_r != diff)
        abort("Noo\n");
    close(ifd);

    // invert temporary array
    for (int i = 0; i < (diff / 2); i++)
    {
        char t = temp[i];
        temp[i] = temp[(diff - 1) - i];
        temp[(diff - 1) - i] = t;
    }

    // move data from temp to final buffer, and shift final buffer -diff- times to the right
    for (int i = 0; i < diff; i++)
    {

        for (int j = filesize; j > i; j--)
            buffer[j] = buffer[j - 1];

        buffer[i] = temp[i];
    }
    free(temp);
}

int main(int argc, char const *argv[])
{
    /* check parameters */
    if (argc != 4)
        abort("usage: invert <source> <target> <number of threads>\n");

    /* opend output file and check errors */
    int ofd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0660);
    if (ofd == -1)
        abort("output creation error\n");

    th_num = atoi(argv[3]);
    filename = argv[1];
    int filesize = getfilesize(filename);

    buffer = calloc(filesize, sizeof(char));
    chunk = filesize / th_num;

    pthread_t ptids[th_num];

    for (int i = 0; i < th_num; i++)
        pthread_create(ptids + i, NULL, invert, (void *)i);

    for (int i = 0; i < th_num; i++)
        pthread_join(ptids[i], NULL);

    int diff = filesize % th_num;
    if (diff != 0)
        // shiftare tutto l'array di n posizioni a destra e riempirlo con i primi n byte di quello originale
        fixRemainingBytes(diff, filesize);

    int size_w = write(ofd, buffer, filesize);
    if (size_w == -1)
        abort("write failed.\n");

    return 0;
}
