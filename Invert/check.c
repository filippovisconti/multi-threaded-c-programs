#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "parameters.h"

void printArrays(char *a1, char *a2, int size)
{

    for (int i = 0; i < size; i++)
        printf("%d ", a1[i]);
    printf("\n");

    for (int i = 0; i < size; i++)
        printf("%d ", a2[size - 1 - i]);
    printf("\n");
}

int main(int argc, char *argv[])
{

    // int size = 64;
    int size = SIZE;
    char a1[size];
    char a2[size];

    int ifd1 = open(INPUT_FILENAME, O_RDONLY);
    int ifd2 = open(OUTPUT_FILENAME, O_RDONLY);

    if (ifd1 < 0 || ifd2 < 0)
        abort("errore");

    int size_r = read(ifd1, a1, size);
    if (size_r == -1)
        abort("read error\n");

    size_r = read(ifd2, a2, size);
    if (size_r == -1)
        abort("read error2\n");

    for (int i = 0; i < size; i++)
        if (a1[i] != a2[size - 1 - i])
        {
            printf("Failed at index %d.\n", i);
            printArrays(a1, a2, size);
            abort("Aborted.\n");
        }
    printf("Correct.\n");
    close(ifd1);
    close(ifd2);
}
