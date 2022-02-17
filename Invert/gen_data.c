#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "parameters.h"

int main(int argc, char *argv[])
{
    int BUFSIZE = SIZE;
    char buffer[BUFSIZE];

    int ofd = open(INPUT_FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 00666);
    int ifd = open("/dev/urandom", O_RDONLY);

    if (ifd < 0 || ofd < 0)
        abort("errore");

    int size_r = read(ifd, buffer, BUFSIZE);
    if (size_r == -1)
        abort("read error\n");

    int size_w = write(ofd, buffer, size_r);
    if (size_w == -1 || size_w != size_r)
        abort("write error\n");

    printf("Written %d bytes in %s.\n", size_w, INPUT_FILENAME);

    close(ifd);
    close(ofd);
}
