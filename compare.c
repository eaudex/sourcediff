#include"compare.h"


double compare_files_sorted(parsed_file file1, parsed_file file2)
{
  double total_hashes, same_hashes;
  long i=0, j=0;

  long f1_num = file1->num_chosen_hashes;
  long f2_num = file2->num_chosen_hashes;

  long long* f1_hashes = file1->chosen_hashes;
  long long* f2_hashes = file2->chosen_hashes;

  /* counter for number of hashes that are equal */
  same_hashes = 0;

  /* determine the larger number of hashes chosen */
  if(f1_num > f2_num)
    total_hashes = f1_num;
  else
    total_hashes = f2_num;

  /* compare the chosen hashes of each file and find similarities */
  while(i < f1_num &&
	j < f2_num)
    {
      if(f1_hashes[i] < f2_hashes[j])
	  i++;
      else if(f1_hashes[i] > f2_hashes[j])
	  j++;
      else
	{
	  same_hashes++;
	  i++;
	  j++;
	}
    }

  /* return ratio of same hashes found */
  return same_hashes / total_hashes;
}


void compare_all_sorted(parsed_file* pfs, repos repo_names, double THRESH)
{
  long i, j;
  double ratio;

  /* compare all files to each other and print those of desired similarity */
  for(i=0; i < repo_names->num_names; i++)
    for(j=i+1; j < repo_names->num_names; j++)
      {
	if(strcmp(repo_names->names[i],"NF") == 0)
	  break;
	if(strcmp(repo_names->names[j],"NF") == 0)
	  continue;
	if((ratio = compare_files_sorted(pfs[i], pfs[j])) > THRESH)
	  printf("%g %s %s\n", ratio, pfs[i]->repo_name, pfs[j]->repo_name);
      }
}



/* legacy code that compares unsorted hashes */

double compare_files(parsed_file file1, parsed_file file2)
{
  long total_hashes, same_hashes;
  long i, j;

  /* counter for number of hashes that are equal */
  same_hashes = 0;

  /* determine the larger number of hashes chosen */
  if(file1->num_chosen_hashes > file2->num_chosen_hashes)
    total_hashes = file1->num_chosen_hashes;
  else
    total_hashes = file2->num_chosen_hashes;

  /* compare the chosen hashes of each file and find similarities */
  for(i=0; i < file1->num_chosen_hashes; i++)
      for(j=0; j < file2->num_chosen_hashes; j++)
	  if(file1->chosen_hashes[i] == file2->chosen_hashes[j])
	    {
	      same_hashes++;
	      break;
	    }
  
  /* return ratio of same hashes found */
  return (double)same_hashes / (double)total_hashes;
}


void compare_all(parsed_file* pfs, repos repo_names, double THRESH)
{
  long i, j;
  double ratio;

  /* compare all files to each other and print those of desired similarity */
  for(i=0; i < repo_names->num_names; i++)
    for(j=i+1; j < repo_names->num_names; j++)
      {
	if(strcmp(repo_names->names[i],"NF") == 0)
	  break;
	if(strcmp(repo_names->names[j],"NF") == 0)
	  continue;
	if((ratio = compare_files(pfs[i], pfs[j])) > THRESH)
	  printf("%g %s %s\n", ratio, pfs[i]->repo_name, pfs[j]->repo_name);
      }
}
