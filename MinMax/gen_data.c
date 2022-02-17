#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "parameters.h"

int main(int argc, char *argv[])
{
    short BUFSIZE = 256;
    short buffer[BUFSIZE];

    int ofd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 00664);
    int ifd = open("/dev/urandom", O_RDONLY);

    if (ifd < 0 || ofd < 0)
        abort("errore");

    int end = 0;
    int total_r = FILE_SIZE;

    while (!end)
    {

        int size_r = read(ifd, buffer, BUFSIZE);
        if (size_r == -1)
            abort("read error\n");
        total_r -= size_r;

        end = total_r == 0;

        int size_w = write(ofd, buffer, size_r);
        if (size_w == -1 || size_w != size_r)
            abort("write error\n");
        printf("written: %d\n", size_w);
    }

    close(ifd);
    close(ofd);
}
