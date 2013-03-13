#include "select.h"

parsed_file select_hashes(parsed_file pf, int mod)
{
  long long* chosen_hashes;
  int i;
  long index = 0;

  chosen_hashes = (long long*)malloc(pf->num_hashes * sizeof(long long));

  for(i=0; i < pf->num_hashes; i++)
    if(pf->hashes[i] % mod == 0)
      chosen_hashes[index++] = pf->hashes[i];

  pf->num_chosen_hashes = index;
  pf->chosen_hashes = chosen_hashes;

  return pf;
}
