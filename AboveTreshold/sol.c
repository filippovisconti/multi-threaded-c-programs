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


int thresholds[NUM_THRESHOLDS] = {0x0, 0x40000000, 0xc0000000};


/* start solution code */

#define CHUNK_SIZE 1024 //num of int to be read in chunk

int fd = 0;
int total = 0;
int th = 0;

void *func(void* args){
  int rb, i, buf[CHUNK_SIZE];

  // read CHUNK_SIZE ints
  while((rb = read(fd,buf,sizeof(int)*CHUNK_SIZE) )>0)    
    
    // iterate over just read ints
    for(i=0;i<rb/sizeof(int);i++)                         
      
      // add 1 to total if buf[i] is higher than threshold
      // this is quite bad for performance. do you know why? how can this be improved?
      __sync_fetch_and_add(&total,buf[i]>th);             

  if(rb<0) printf("error while reading\n");

  return NULL;
}

/* end solution code */

int above_threshold(char *filename, int count, int threshold){
/* start solution code */

   // get a file descript and store on a variable easily accessible for child threads
   fd = open(filename, O_RDONLY);
   // put threshold in a global variable
   th = threshold;
   // init a shared counter
   total = 0;

   if(fd<0) printf("error while opening\n");
   
   // allocate tids for pthread_join
   pthread_t *tids = malloc(count*sizeof(pthread_t));
   
   int i;
   // create count threads
   for(i=0;i<count;i++) pthread_create(tids+i, NULL, func, NULL);
   // wait each child
   for(i=0;i<count;i++) pthread_join(tids[i], NULL);
   // close file
   close(fd);
   // release allocated tids
   free(tids);
   // print and return result as requested
   printf("%d\n", total);
   return total;
/* end solution code */
}


int main(){
  int res;
  int i;
  for(i=0;i<NUM_THRESHOLDS;i++){
    res = above_threshold(FILENAME, NUM_THREADS, thresholds[i]);
    printf("The correct answer for %d is %d\n", thresholds[i], res);
  }
}

