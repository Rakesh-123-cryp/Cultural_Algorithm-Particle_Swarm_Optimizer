#include <iostream>
#include <omp.h>

//Merging 2 sorted subarrays into one tmp array
void merge(int * X, int n, int * tmp) {
   int i = 0;
   int j = n/2;
   int ti = 0;
	//i will iterate till first  half anf J will iterate for 2nd half of array
   while (i<n/2 && j<n) {
      if (X[i] < X[j]) {
         tmp[ti] = X[i];
         ti++; i++;
      } else {
         tmp[ti] = X[j];
         ti++; 
	 j++;
      }
   }
   while (i<n/2) { /* finish up lower half */
      tmp[ti] = X[i];
	ti++;
	i++;
   }
      while (j<n) { /* finish up upper half */
         tmp[ti] = X[j];
         ti++; 
	 j++;
   }
	//Copy sorted array tmp back to  X (Original array)
   memcpy(X, tmp, n*sizeof(int));

} // end of merge()

void mergesort(int * X, int n, int * tmp)
{
   if (n < 2) return;

   #pragma omp task firstprivate (X, n, tmp)
   mergesort(X, n/2, tmp);

   #pragma omp task firstprivate (X, n, tmp)
   mergesort(X+(n/2), n-(n/2), tmp);
	
   //Wait for both paralel tasks to complete execution
   #pragma omp taskwait

    /* merge sorted halves into sorted list */
   merge(X, n, tmp);
}