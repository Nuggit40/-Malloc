
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"
  
int
main ()
{
  
struct timeval current_time, timeA, timeB, timeC, timeD, timeE;
  
gettimeofday (&current_time, NULL);
  
srand (time (0));
  
double last_time_interval = current_time.tv_usec;
  
double loadA[50];
  
double loadB[50];
  
double loadC[50];
  
double loadD[50];
  
double loadE[50];
  
double totalA = 0;
  
double totalB = 0;
  
double totalC = 0;
  
double totalD = 0;
  
double totalE = 0;
  
double avgA = 0;
  
double avgB = 0;
  
double avgC = 0;
  
double avgD = 0;
  
double avgE = 0;
  
int big_count = 0;
  
while (big_count < 50)
    {
      
int free_counter = 0;
      
int malloc_counter = 0;
      
int counter = 0;
      
int counter1 = 0;
      
int top = 0;
      
int i = 0;
      
int mallocCount = 0;
      
int freeCount = 0;
// Workload A mallocing a byte and immediatley freeing it
while (counter < 120)
	{
	  
 
	    //function pointer to malloc calling malloc 1 byte
	  char *r = (char *) malloc (1);
	  
	    //function pointer to free to release byte
	    free (r);
	  
counter += 1;
	
} 

//calculating time of loop
gettimeofday (&timeA, NULL);
      
loadA[big_count] = (timeA.tv_usec) - (current_time.tv_usec);
      
last_time_interval = (current_time.tv_usec);
      
counter = 0;
      
char *arr[120];
// Workload B " malloc() 1 byte, store the pointer in an array - do this 120 times."     
while (counter < 120)
	{
	  
	    //function pointer to malloc calling malloc 1 byte
	    arr[counter] = (char *) malloc (1);
	  
counter += 1;
	
} 
counter = 0;
      
while (counter < 120)
	{
	  
	    //function pointer to free to release byte
	    free (arr[counter]);
	  
counter += 1;
	
}
      
counter = 0;
//calculating time of loop
gettimeofday (&timeB, NULL);
      
loadB[big_count] = (timeB.tv_usec) - (current_time.tv_usec);
      
	
	last_time_interval = current_time.tv_usec;
      
char *pointers[120];
      
//Workload C  240 times, randomly choose between a 1 byte malloc() or free()ing one of the malloc()ed pointers

	for (i; i < 240; ++i)
	{
	  
int r = rand () % 2;
	  
if (mallocCount < 120 && r == 0)
	    {
	      
		//space to malloc again
		pointers[top] = (char *) malloc (1);
	      
++top;
	      
++mallocCount;
	      
 
		
	    }
	  else if (freeCount < 120 && r == 1 && top != 0)
	    {
	      
		//pointers to free again
		free (pointers[top - 1]);
	      
--top;
	      
++freeCount;
	      
		
	    }
	  else if (r == 0)
	    {
	      
		//no more mallocs needed, do a free
		free (pointers[top - 1]);
	      
--top;
	      
++freeCount;
	      
		
	    }
	  else if (r == 1)
	    {
	      
		//no more pointers to free, do a malloc
		pointers[top] = (char *) malloc (1);
	      
++top;
	      
++mallocCount;
	      
		
	    }
	
} 
//calculating time of loop
gettimeofday (&timeC, NULL);
      
loadC[big_count] = (timeC.tv_usec) - (current_time.tv_usec);
      
	
	last_time_interval = current_time.tv_usec;
      
counter = 0;

// Workload D attempt at over malloc
while (counter < 120)
	{
	  
char *r = (char *) malloc (5000);
	  
free (r);
	  
counter += 120;
	
} 
counter = 0;
// calculating time of loop
gettimeofday (&timeD, NULL);
      
loadD[big_count] = (timeD.tv_usec) - (current_time.tv_usec);
      
	
	last_time_interval = current_time.tv_usec;


//Workload E malloc 0 and attempt to free with no bytes malloced      
while (counter < 120)
	{
	  
char *r = (char *) malloc (0);
	  
free (r);
	  
counter += 120;
	
} 
// calculating time of loop
gettimeofday (&timeE, NULL);
      
loadE[big_count] = (timeE.tv_usec) - (current_time.tv_usec);
      
	
	last_time_interval = current_time.tv_usec;
      
 

      
big_count += 1;
    
} 
int j = 0;
  
while (j < 50)
    {
      
totalA += loadA[j];
      
totalB += loadB[j];
      
totalC += loadC[j];
      
totalD += loadD[j];
      
totalE += loadE[j];
      
j += 1;
    
}
  
avgA = totalA / 50;
  
avgB = totalB / 50;
  
avgC = totalC / 50;
  
avgD = totalD / 50;
  
avgE = totalE / 50;
  
printf ("total time of workload A is %lf microseconds with an average of %lf microseconds\n", totalA,
	   avgA);
  
printf ("total time of workload B is %lf microseconds with an average of %lf microseconds\n", totalB,
	   avgB);
  
printf ("total time of workload C is %lf microseconds with an average of %lf microseconds\n", totalC,
	   avgC);
  
printf ("total time of workload D is %lf microseconds with an average of %lf microseconds\n", totalD,
	   avgD);
  
printf ("total time of workload E is %lf microseconds with an average of %lf microseconds\n", totalE,
	   avgE);
  
 
return 0;

}


 
 
