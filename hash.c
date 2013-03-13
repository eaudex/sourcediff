#include"hash.h"

parsed_file hash_text(parsed_file pf, int ss_length)
{
  long i, j;
  long long* hashes;
  long long hash_mask, mask, last_hash, cur_hash;

  /* turn on last byte of the mask */
  mask = 0xff;

  hash_mask = 0;
  cur_hash = 0;

  /* turn on as many bytes as necessary for the given substring length */
  for(i=0; i < ss_length; i++)
    {
      hash_mask += mask;
      mask = mask << 8;
    }

  /* computes total number of hashes to be calculated */
  pf->num_hashes = pf->length - ss_length + 1;

  /* allocate memory for an array of all hash values */
  hashes = (long long*)malloc(pf->num_hashes * sizeof(long long));
  
  /* compute the initial hash value */
  /* sets each byte of a long long var equal to one byte of the substring */
  for(i=0; i < ss_length; i++)
    cur_hash += (long long)pf->text[i] << (8 * (ss_length - i - 1));

  hashes[0] = cur_hash;

  /* rolling hash function to compute the remaining hash values */
  for(i=1; i < pf->num_hashes; i++)
    {
      /* shift all bytes over one byte */
      cur_hash = cur_hash << 8;
      /* set last byte equal to new val from string */
      cur_hash += pf->text[i + ss_length - 1];
      /* filter hash using mask so only given number of bytes are set */
      cur_hash &= hash_mask;
      hashes[i] = cur_hash;
    }

  pf->hashes = hashes;

  return pf;
}
 

/* hash all of the repos and return them in an array for comparison */
parsed_file* hash_repos(repos repo_names, char* PATH, char* filename,
			int BUF_SIZE, int WHITESPACE, int SORT, 
			int MOD_SELECTOR, int SS_LENGTH, int PARSE_ALL)
{
  int i;
  FILE* fp;
  parsed_file pf, *pfs;
  char command[BUF_SIZE];
  char* cat_file;
  

  /* allocate memory for an array of the parsed files */
  pfs = (parsed_file*)malloc(repo_names->num_names * sizeof(parsed_file));

  /* create temp file to hold concatenated files */
  cat_file = mktmp();

  /* concatenate all requested file, then read in all data, hash it, select */
  /* which hashes to use, then sort and return an array of all parsed files */
  for(i=0; i < repo_names->num_names; i++)
    {
      /* create the command string the shell will execute */
      sprintf(command, "cat %s/%s/%s 1>%s 2>/dev/null", PATH, repo_names->names[i], 
	      filename, cat_file);
      
      /* excute command to concatenate all requested file */
      system(command);


      /* open file and print err message if there is no file */
      if((fp = fopen(cat_file, "r")) == NULL)
	{
	  fprintf(stderr, "No files: %s/%s/%s\n", PATH, 
		  repo_names->names[i], filename);
	  repo_names->names[i] = strdup("NF");
	  continue;
	}

      /* parse the file if it exists */
      pf = parse(fp, WHITESPACE, PARSE_ALL);

      /* if no valid chars in file do not compare and print err msg */
      if(pf == 0)
	{
	  fprintf(stderr, "Empty file: %s/%s\n", PATH, 
		  repo_names->names[i], filename);
	  repo_names->names[i] = strdup("NF");
	  continue;
	}

      pf->repo_name = repo_names->names[i];
      pf = hash_text(pf, SS_LENGTH);
      pf = select_hashes(pf, MOD_SELECTOR);

      /* pf->selected hashes will be used from here on out so free pf->hashes */
      free(pf->hashes);

      if(SORT)
	quicksort(pf->chosen_hashes, pf->num_chosen_hashes);
      pfs[i] = pf;
    }

  /* remove temp file which is no longer necessary */
  sprintf(command, "rm %s", cat_file);
  system(command);

  return pfs;
}
