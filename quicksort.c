#include "quicksort.h"

/* swap two hashes in an array */
void swap(long long* a, long long* b)
{
  long long temp;
  temp = *a;
  *a = *b;
  *b = temp;
  return;
}

/* find the final index given the array, and left and right most indices */
void sort(long long* array, long left, long right)
{
  long long pivot = array[right];
  long newInd=left;
  long i;
  if (left >= right || right <= left)
    return;

  /* swap all elements less than the pivot to the beginning of the array */
  for(i=left; i<right; i++)
      if (array[i] < pivot)
	swap(&array[i], &array[newInd++]);

  /* swap the pivot value into its final location */
  swap(&array[right], &array[newInd]);

  /* sort elements less than the pivot */
  sort(array, left, newInd-1);

  /* sort elements greater than the pivot */
  sort(array, newInd+1, right);

  return;
}

/* side effect of sorting an array given its length */
void quicksort(long long* array, long len)
{
  sort(array, 0, len-1);
  return;
}
