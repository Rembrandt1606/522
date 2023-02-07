#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include<unistd.h>
#define ITER   10
#define MAX_N 64*1024*1024 
#define MAX_ARR 64*1024*1024 
#define MB    (1024*1024)
#define KB (1*1024)
// LLC Parameters assumed
#define START_SIZE 512*KB
#define STOP_SIZE  1024*MB
#define SIZE 128
#define BILLION 1000000000L
char array[MAX_ARR];
// Provides elapsed Time between t1 and t2 in milli sec

double elapsedTime(struct timeval t1, struct timeval t2){
  double delta;
  delta = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  delta += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  return delta; 
}

// Fisher Yates suffle
// SOURCE: https://stackoverflow.com/questions/42321370/fisher-yates-shuffling-algorithm-in-c
void FisherYates(int *player, int n) { //implementation of Fisher

    int i, j, tmp; // create local variables to hold values for shuffle

    for (i = n - 1; i > 0; i--) { // for loop to shuffle
         j = rand() % (i + 1); //randomise j for shuffle with Fisher Yates
         tmp = player[j];
         player[j] = player[i];
         player[i] = tmp;
    }
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
    // When the performance between successive iterations is not different, the processor is limited by cache access latency
    // Meaning a new 
      if(PercentDiff(retvec[i], retvec[i+1]) < .3){
        retval = pow(2.0,(double)i+1);
      }
  }
 
    free(retvec);
    
  return retval; 
}
/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////
float CacheSizeTest(int line_size)
{    
  //Defines the # of doublings until stop size is reach (starting at 256KB ) 
  int div = (STOP_SIZE/(512 * KB));  
  //Log base 2 of this number
  int iter = (int)log_2(div) + 1;   
  //The current size of our test array
  int size = 0;                      
  //Our test array, using ints to make calcuation simpler (1G size)
  int *testr = (int *)malloc(1024*MB * sizeof(int)); 
  //Number of steps to get accurate estimation of access time
  int steps = 20 * 1024 * 1024; 
  //Array to store the execution times
  double *retvec = (double *)calloc(iter, sizeof(double));
  float *sizevec = (float *)calloc(iter, sizeof(float));
  // DEFINING OUR TIME VARIABLES
  long long unsigned int time_diff = 0;
  double time;
  float report_size = 0.0;
  struct timeval t1, t2;
  int estimate_found = 0;
  
  // TEST ITSELF
  for (int j = 0; j<iter; j++) {
    
    size = pow(2,j)*START_SIZE - 1;
    report_size = (float)size/(1*MB);
    //printf("current size is %.1f MB \n", report_size);
    sizevec[j] = report_size;
    gettimeofday(&t1, NULL);
    for(int i = 0; i<steps;i++){
      testr[(i * (line_size/sizeof(int))) % size]++;
    }
    gettimeofday(&t2, NULL);
    time = elapsedTime(t1,t2);
    retvec[j] = time;
    //printf("Time: %lf \n", time);
    //printf("Average time per element: %lf us\n", (double)(time/(double)steps) * 1000);
  }
  float retval = 0.0;
  for(int i = 1; i<iter-1; i++){
    //printf("Testing at %d iteration \n", i);
    // When the performance between successive iterations is not different, the processor is limited by access latency
      if(PercentDiff(retvec[i], retvec[i-1]) > 3 && estimate_found == 0){
        retval = sizevec[i];
        //printf("Estimate found at %d iteration with value of %.1f MB \n", i, retval);
        estimate_found = 1;
        
      }
  }
 
  free(testr);
  free(retvec);
  free(sizevec);
  return retval; 
}
/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////
void MemoryTimingTest(void)
{    
  int *testr = (int *)malloc(16 * sizeof(int));
  struct timespec start, end;
  u_int64_t diff;

  clock_gettime(CLOCK_MONOTONIC, &start);	/* mark start time */
	testr[0] = 1;
	clock_gettime(CLOCK_MONOTONIC, &end);	/* mark the end time */

  diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("[INFO] Estimator for memory access latency is = %llu nanoseconds\n", (long long unsigned int) diff);

  free(testr); 
}
/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////
double CacheAssocTest(void)
{    
  // Didnt get too :(
  double retval;
  return retval; 
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
int main(){
  
  printf("Starting Test:\n");
  int line_size = (int)LineSizeTest();
  printf("[INFO] Cache Line Size: %d bytes \n", line_size);
  float llc_size = CacheSizeTest(line_size);
  printf("[INFO] Best Estimator of LLC size: %.1f MB \n", llc_size);
  MemoryTimingTest();

  
  // Add your code here, and comment above
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
