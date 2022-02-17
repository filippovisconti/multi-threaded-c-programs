#ifndef __PARAMETERS__
#define __PARAMETERS__

#define abort(msg)   \
    do               \
    {                \
        printf(msg); \
        exit(1);     \
    } while (0)

#define INPUT_FILENAME "dataset.bin"
#define OUTPUT_FILENAME "inverted.bin"
#define SIZE 4096

#endif