#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define FILENAME "dataset.bin"
#define NUM_THREADS 4
#define NUM_THRESHOLDS 3
#define CHUNK 1024

int threshold = 0;
int total_count = 0;
int fd = -1;
pthread_spinlock_t ptspin;

int thresholds[NUM_THRESHOLDS] = {0x0, 0x40000000, 0xc0000000};

/*
This function must launch a number of threads / processes equal
to n to read a sequence of ints from the binary file filename and
count how many of these have a value greater than threshold.
Finally, the function prints to standard output and returns the
total number of int greater than threshold value contained in the
file.
*/

void *counter(void *param)
{
  int count = 0;
  int size_r = 0;
  // lock to read
  do
  {
    int *array = calloc(1, CHUNK);

    pthread_spin_lock(&ptspin);
    size_r = read(fd, array, CHUNK);
    pthread_spin_unlock(&ptspin);

    for (int i = 0; i < size_r / sizeof(int); i++)
    {
      if (array[i] > threshold)
        count++;
    }
  } while (size_r != 0);

  // atomic instruction to update value
  __sync_fetch_and_add(&total_count, count);
}

int above_threshold(char *filename, int count, int threshold1)
{
  // generate -count- threads

  total_count = 0;
  threshold = threshold1;

  pthread_t ptids[count];
  for (int i = 0; i < count; i++)
    pthread_create(ptids + i, NULL, counter, NULL);

  for (int i = 0; i < count; i++)
    pthread_join(ptids[i], NULL);

  return total_count;
}

int main()
{
  int res, i;

  for (i = 0; i < NUM_THRESHOLDS; i++)
  {
    pthread_spin_init(&ptspin, PTHREAD_PROCESS_PRIVATE);

    fd = open(FILENAME, O_RDONLY);

    res = above_threshold(FILENAME, NUM_THREADS, thresholds[i]);
    printf("The correct answer for %d is %d\n", thresholds[i], res);

    close(fd);
    fd = -1;

    pthread_spin_destroy(&ptspin);
  }
}
