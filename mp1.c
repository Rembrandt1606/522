#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#define ITER   10
#define MAX_N 64*1024*1024 
#define MB    (1024*1024)
// LLC Parameters assumed
#define START_SIZE 1*MB
#define STOP_SIZE  16*MB
#define SIZE 64
char array[MAX_N];
/////////////////////////////////////////////////////////
// Provides elapsed Time between t1 and t2 in milli sec
/////////////////////////////////////////////////////////
double elapsedTime(struct timeval t1, struct timeval t2){
  double delta;
  delta = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  delta += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  return delta; 
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
double DummyTest(void)
{    
  struct timeval t1, t2;
  int ii, iterid;
  // start timer
  gettimeofday(&t1, NULL);
  for(iterid=0;iterid<ITER;iterid++){
    for(ii=0; ii< MAX_N; ii++){
      array[ii] += rand();
    }
  }
  // stop timer
  gettimeofday(&t2, NULL);
 
  return elapsedTime(t1,t2);
}
/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////
double LineSizeTest(void)
{    
    struct timeval t1, t2;
    int *vec = (int *)malloc(sizeof(int) * SIZE);
    double *retvec = (double *)calloc(sizeof(int) * SIZE, sizeof(double));
    unsigned char byte;

    for(int i = 0; i<SIZE; i++)
    {
        for(int j = 0; j<sizeof(int); j++) {
            gettimeofday(&t1, NULL);
            byte = (vec[i] >> (8*j)) & 0xff;
            gettimeofday(&t2, NULL);
            printf("elapsedTime is: %lf \n", elapsedTime(t1,t2));
            retvec[i + j] = elapsedTime(t1,t2);
        }

    }
    for(int i = 0; i<10; i++){
        printf("Time at %d iteration %lf \n", i, retvec[i]);
    }
    free(vec);
    free(retvec);
    double retval = 0.0;
  return retval; 
}
/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////
double CacheSizeTest(void)
{    
  double retval;
  return retval; 
}
/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////
double MemoryTimingTest(void)
{    
  double retval;
  return retval; 
}
/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////
double CacheAssocTest(void)
{    
  double retval;
  return retval; 
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
int main(){
  
  printf("Starting Test:\n");
  printf("Test took %lf seconds\n", LineSizeTest()/1000.0);

  // Add your code here, and comment above
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
