#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include "structs.h"
#include "parser.h"
#include "hash.h"
#include "select.h"
#include "compare.h"
#include "quicksort.h"
#include "mktmp.h"

/* default values */
int SS_LENGTH = 6;
int MOD_SELECTOR = 1;
double THRESH = 0;
int SORT = 1;
int WHITESPACE = 0;
long NAME_BUF_SIZE = 1000;
int BUF_SIZE = 256;
int PARSE_ALL = 0;


/* to be set when parsing command line args */
char* PATH = NULL;
char* filename = NULL;

/* help: prints the help message and possible input options */
void help()
{
  printf("Usage: sourcediff [OPTIONS] [PATH] [FILE-NAMES]\n");
  printf("Options:\n\n");
  printf("-a\t\tparse all characters in all files.\n\n");
  printf("-h\t\tdisplay this help and exit.\n\n");
  printf("-l [num]\tsubstring length to be tested [1-8].\n"
	 "\t\tDEFAULT: %i\n\n", SS_LENGTH);
  printf("-m [num]\tmod value used when selecting which hashes to compare.\n"
	 "\t\tuse 1 to compare all hash values.\n"
	 "\t\tDEFAULT: %i\n\n", MOD_SELECTOR);
  printf("-t [num]\tprint files above this similarity percentage.\n"
	 "\t\tinput must be an integer.\n"
	 "\t\tDEFAULT: %g\n\n", THRESH * 100);
  printf("-w\t\tinclude whitespace in comparison analysis.\n\n");
  printf("-s\t\tturn off sorting of hashes before comparing.\n"
	 "\t\t(for testing purposes)\n\n");
  exit(1);
}

int options(int argc, char* argv[])
{
  char c;
  opterr = 0;
     
  while ((c = getopt (argc, argv, "ahl:m:st:w")) != -1)
    switch (c)
      {
      case 'h':
	help();
	break;
      case 'l':
	SS_LENGTH = atoi(optarg);
	break;
      case 'm':
	MOD_SELECTOR = atoi(optarg);
	break;
      case 't':
	THRESH = (double)atoi(optarg) / (double)100;
	break;
      case 's':
	SORT = 0;
	break;
      case 'w':
	WHITESPACE = 1;
	break;
      case 'a':
	PARSE_ALL = 1;
	break;
      case '?':
	if (optopt == 'c' || optopt == 't' || optopt == 'm')
	  fprintf (stderr, "sourcediff: Option -%c "
		   "requires a number argument.\n", optopt);
	else if (isprint (optopt))
	  fprintf (stderr, "sourcediff: Unknown option `-%c'.\n", optopt);
	else
	  fprintf (stderr,
		   "Unknown option character `\\x%x'.\n",
		   optopt);
	exit(1);
      default:
	help();
      }

  /* print help when user enters `soucediff help' */
  if(argv[optind] != NULL && strcmp(argv[optind],"help") == 0)
    help();
  
  /* error if no path or file names */
  if(argv[optind] == NULL || argv[optind+1] == NULL)
    {
      fprintf(stderr, "sourcediff: must include path and file names.\n");
      exit(1);
    }

  /* read PATH and file names (required variables) */
  PATH = argv[optind];
  filename = argv[optind+1];

  return 0;
}



int main(int argc, char* argv[])
{
  parsed_file pf, *pfs;
  repos repo_names; 
  char command[BUF_SIZE];
  long i,j;
  FILE* fp;
  char* repo_fn;

  /* read command line args */
  options(argc, argv);
  
  /* create a temp file with all repo names */
  repo_fn = create_repo_name_file(PATH);

  /* open and read the repo names into a local data struct */
  /* print error if file not found */
  if ((fp = fopen(repo_fn, "r")) == NULL)
    {
      fprintf(stderr,"No file named: %s\n", repo_fn);
      exit(1);
    }
  
  /* read in all repo names to a local structure */
  repo_names = read_repo_names(fp, NAME_BUF_SIZE);

  /* remove temp file which is no longer necessary */
  sprintf(command, "rm %s", repo_fn);
  system(command);

  /* hash all of the repos and return them in an array for comparison */
  pfs = hash_repos(repo_names, PATH, filename, BUF_SIZE, WHITESPACE,
		   SORT, MOD_SELECTOR, SS_LENGTH, PARSE_ALL);


  /* print the results of the analysis */
  if(SORT)
    compare_all_sorted(pfs, repo_names, THRESH);
  else
    compare_all(pfs, repo_names, THRESH);
  
  return 0;
}
