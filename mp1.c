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
#define L1SIZE 32*KB
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
  int max_iter = (int)log_2((STOP_SIZE/KB)) + 1;
  double *retvec = (double *)calloc(max_iter, sizeof(double));
  int access;
  int num_accesses = 0;
  struct timeval t1, t2;
  struct timespec start, end;
  int num_iters;
  long long unsigned int run_sum = 0;
  int testr = 0.0;
  printf("[INFO] Max number of steps is: %d \n", max_iter);
  
  for(int i = 0; i<max_iter; i++){
    
    current_size = KB * (int)pow(2.0,i);
    num_iters = 0;
    run_sum = 0;
    num_accesses = current_size / line_size;
    // Pre-cache array addresses and ensure early address are cached
   /* srand(i+1);
    for (int ii = MAX_ARR-1; ii > MAX_ARR - num_accesses - 1; ii--) { // for loop to shuffle
        
        testr = rand() % (ii + 1); //randomise j for shuffle with Fisher Yates
        access = testr/line_size;
        array[access] = 0;
        //printf("[INFO] Access at: %d \n", access);
    }*/

    
    gettimeofday(&t1, NULL);
    for (int k = 0; k<400; k++){
      srand(i+1);
      for (int ii = MAX_ARR-1; ii > MAX_ARR - num_accesses - 1; ii--)
      {
          testr = rand() % (ii + 1); //randomise j for shuffle with Fisher Yates
          access = testr/line_size;
          //printf("[INFO] Accessing at %d index \n", access);
          clock_gettime(CLOCK_MONOTONIC, &start);
          array[access] += 1;
          clock_gettime(CLOCK_MONOTONIC, &end);
          run_sum += BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
          num_iters++;
      }
   }
    gettimeofday(&t2, NULL);
    printf("----------------------------------------------------------- \n \n");
    printf("[INFO] ElapsedTime is: %lf ms \n", elapsedTime(t1,t2));
    //printf("[INFO] The average runtime is: %llu nanoseconds \n", (run_sum/3));
    //printf("[INFO] The number of element accessed is: %d \n", num_iters);
    //printf("[INFO] The address range is: %d bytes \n", (num_iters)*line_size);
    printf("[INFO] Number of accesses is: %d \n", num_accesses);
    printf("[INFO] The current size is: %d KB \n", (int)pow(2.0,i));
    //printf("[INFO] The average access time is: %llu nanoseconds \n", (run_sum/num_iters));
    retvec[i] = elapsedTime(t1,t2);
  }

  double retval;

  /*for(int i = 0; i<max_iter-1; i++){ 
    printf("Percent diff between %lf and %lf is %lf \n", retvec[i], retvec[i+1], PercentDiff(retvec[i], retvec[i+1]));
      if(PercentDiff(retvec[i], retvec[i+1]) > .3){
        //retval = pow(2.0,(double)i+1);
        printf("Different at %d\n", i+1);
      }
  }*/
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
  //double testr = CacheSizeTest(line_size);  

  int number_of_CL = L1SIZE/line_size;
  printf("[INFO] The number of cache lines in L1 is: %d lines \n", number_of_CL);

  printf("Lets create an array with this 32x the number of lines in our L1\n");

  int *testr = (double *)malloc(number_of_CL * 32 * sizeof(int));
  long long unsigned int time_diff = 0;
  printf("int *testr = (double *)malloc(number_of_CL * 8 * sizeof(int));\n");
  printf("Lets create an array with this 8x the number of lines in our L1\n");

  printf("Lets read one of these lines from memory and measure it latency\n"); 
  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);
  array[0] = 0;
  clock_gettime(CLOCK_MONOTONIC, &end);
  time_diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

  printf("This access took %llu nanoseconds\n", time_diff);
  free(testr);
  // Add your code here, and comment above
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
