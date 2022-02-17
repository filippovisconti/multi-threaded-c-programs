#ifndef __PARAMETERS__
#define __PARAMETERS__

#define abort(msg)   \
    do               \
    {                \
        printf(msg); \
        exit(1);     \
    } while (0)

#define SIZE 256
#define COUNT 8  // number of file descriptors and threads
#define CHUNK 16 // number of bytes per chunk
#define STDOUT 1

#endif
