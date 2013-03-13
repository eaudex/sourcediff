#include"winnower.h"

parsed_file winnow(parsed_file pf, int window_size)
{
  long long min, index, *chosen_hashes, cur_hash;
  long i;
  int found;
  /* initialize minimum var to first hash value */
  min = pf->hashes[0];

  index = 0;
  cur_hash = 0;

  /* allocate memory for the hashes chosen based on winnowing algorithm */
  chosen_hashes = (long long*)malloc(pf->num_hashes * sizeof(long long));

  /* determine min value in first window */

  while(index < pf->num_hashes)
    {
      found = 0;
      /* finds the next smallest hash in the given window if one exists */
      for(i=1; i < window_size && i+index < pf->num_hashes; i++)
	{
	  if(pf->hashes[i + index] <= min)
	    {
	      min = pf->hashes[i + index];
	      index += i;
	      found = 1;
	      continue;
	    }
	}
      if(found == 0)
	chosen_hashes[cur_hash++] = min;
      /* if no value in the window is less than the current hash then */
      /* find the min value of the window directly after the current hash */
      /* and set it as the new min value */
      else
	index++;
      /*
      else
	{
	  index++;
	  min = pf->hashes[index];
	  for(i=1; i < window_size && i+index < pf->num_hashes; i++)
	    {
	      if(pf->hashes[i + index] <= min)
		{
		  min = pf->hashes[i + index];
		  index += i;
		}
	    }
	  chosen_hashes[cur_hash++] = min;
	}
      */
    }

  pf->num_chosen_hashes = cur_hash;
  pf->chosen_hashes = chosen_hashes;

  return pf;
}
