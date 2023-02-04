#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#define ITER   10
#define MAX_N 64*1024*1024 
#define MB    (1024*1024)
// LLC Parameters assumed
#define START_SIZE 1*MB
#define STOP_SIZE  16*MB
#define SIZE 128
#define BILLION 1000000000L
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

  int num_steps = 10;
  double *retvec = (double *)calloc(num_steps, sizeof(double));

  for(int i = 0; i<num_steps; i++){
    struct timeval t1, t2;
    double val = pow(2.0,(double) i);
    int iter = (int)val;
    printf("[INFO] val is %d \n", iter);
    gettimeofday(&t1, NULL);
    for(int k=0; k< MAX_N; k+=iter)
      {
        array[k] *= 2;
      }
    gettimeofday(&t2, NULL);
    printf("[INFO] At iter %d elapsedTime is: %lf S \n", i, elapsedTime(t1,t2));
  }
  /*
    struct timespec start, end;
    int *vec = (int *)malloc(sizeof(int) * SIZE);
    double *retvec = (double *)calloc(sizeof(int) * SIZE, sizeof(double));
    int val;
    u_int64_t diff;
    for(int i = 0; i<SIZE; i++)
    {
    clock_gettime(CLOCK_MONOTONIC, &start);
    val = vec[i] * 3;
    clock_gettime(CLOCK_MONOTONIC, &end);
    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("[INFO] At iter %d elapsedTime is: %llu nS \n", i+1, (long long unsigned int) diff);
        //for(int j = 0; j<sizeof(int); j++) {
            
        //}
    printf("_______________________________________________ \n");

    }
    //for(int i = 0; i<10; i++){
    //   printf("Time at %d iteration %lf \n", i, retvec[i]);
   // }
    free(vec);
    free(retvec);
    */
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
