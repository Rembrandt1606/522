#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#define ITER   10
#define MAX_N 64*1024*1024 
#define MAX_ARR 1024*1024*1024 
#define MB    (1024*1024)
#define KB  1024
// LLC Parameters assumed
#define START_SIZE 1*MB
#define STOP_SIZE  16*MB
#define SIZE 128
#define BILLION 1000000000L
char array[MAX_ARR];
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

double PercentDiff(double x, double y){
  return (abs(x-y)/((x+y)/2))*100;
}

double log_2(double x){
  return (log(x)/log(2));
}
/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////
double LineSizeTest(void)
{    

  int num_steps = 10;
  double *retvec = (double *)calloc(num_steps, sizeof(double));
  struct timeval t1, t2;

  gettimeofday(&t1, NULL);
  for(int k=0; k< MAX_N; k++)
      {
        array[k] = 5;
      }
  gettimeofday(&t2, NULL);
  for(int i = 0; i<num_steps; i++){
    
    double val = pow(2.0,(double) i);
    int iter = (int)val;
    gettimeofday(&t1, NULL);
    for(int k=0; k< MAX_N; k+=iter)
      {
        array[k] *= 2;
      }
    gettimeofday(&t2, NULL);
    //printf("[INFO] At step size %d elapsedTime is: %lf ms \n", iter, elapsedTime(t1,t2));
    retvec[i] = elapsedTime(t1,t2);
  }
  double retval = 0.0;
  for(int i = 0; i<num_steps-1; i++){
    //printf("Percent diff between %lf and %lf is %lf \n", retvec[i], retvec[i+1], PercentDiff(retvec[i], retvec[i+1]));
      if(PercentDiff(retvec[i], retvec[i+1]) < .3){
        retval = pow(2.0,(double)i+1);
        //printf("Not Different at %d\n", i+1);
      }
  }
 
    free(retvec);
    
  return retval; 
}
/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////
double CacheSizeTest(int line_size)
{    

  int current_size = 0;
  int max_iter = (int)log_2((MAX_ARR/KB)) + 1;
  double *retvec = (double *)calloc(max_iter, sizeof(double));
  struct timeval t1, t2;
  printf("[INFO] Max number of steps is: %d \n", max_iter);

  for(int i = 0; i<max_iter; i++){
    
    current_size = KB * (int)pow(2.0,i);
    // Pre-cache array
    for (int j=0; j<current_size; j+=line_size)
    {
        array[j] = 0;
    }

    gettimeofday(&t1, NULL);
    for (int j=0; j<current_size; j+=line_size)
    {
        array[j] = 0;
    }
    gettimeofday(&t2, NULL);
    printf("[INFO] At step size %d elapsedTime is: %lf ms \n", current_size, elapsedTime(t1,t2));
    retvec[i] = elapsedTime(t1,t2);
  }

  double retval;

  for(int i = 0; i<max_iter-1; i++){ 
    printf("Percent diff between %lf and %lf is %lf \n", retvec[i], retvec[i+1], PercentDiff(retvec[i], retvec[i+1]));
      if(PercentDiff(retvec[i], retvec[i+1]) < .3){
        //retval = pow(2.0,(double)i+1);
        printf("Not Different at %d\n", i+1);
      }
  }
  free(retvec);
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
  int line_size = (int)LineSizeTest();
  printf("[INFO] Cache Line Size: %d bytes \n", line_size);
  double testr = CacheSizeTest(line_size); 
  // Add your code here, and comment above
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
