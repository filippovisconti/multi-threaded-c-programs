#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "parameters.h"

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

    for (int i = 0; i < count; i++)
    {
        int fd = open(fnames[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);

        char buffer[SIZE];
        for (int j = 0; j < SIZE; j++)
            buffer[j] = 'a' + i;

        int size_w = write(fd, buffer, SIZE);
        if (size_w < 0)
            abort("Write failed.\n");

        close(fd);
    }
}