#include "parser.h"

/* determines the length of a given file */
long file_length(FILE* fp)
{
  long length;

  /* sets file position to the end of file */
  fseek(fp, 0, SEEK_END);

  length = ftell(fp);

  /* sets file position to beginning of file */
  rewind(fp);
  
  return length;
}

parsed_file parse(FILE* fp, int WHITESPACE, int PARSE_ALL)
{
  long i;
  char *text, *input;
  char c;
  parsed_file pf;

  /* allocate memory for the parsed file structure */
  pf = (parsed_file)malloc(sizeof(struct parsed_file));

  pf->length = file_length(fp);
  /* if there is a file containing nothing */
  if(pf->length == 0)
    return 0;
 
  /* allocate memory for file to be read into */
  input = (char*)malloc((pf->length + 1) * sizeof(char));

  /* read file into an array one character at a time */
  for(i=0; (c=fgetc(fp)) != EOF;)
    {
      /* if the char is a letter/num, insert in array and incr counter */
      if ((c <= 'z' && c >= 'a') || 
	  (c <= '9' && c >= '0'))
	input[i++] = c;
      else if(WHITESPACE && c == ' ')
	input[i++] = c;
      else if (c <= 'Z' && c >= 'A')
	input[i++] = c + 32;
      else if (PARSE_ALL)
	input[i++] = c;
    }
  input[i] = '\0';
  
  /* i is equal to the total number of relevant characters */
  pf->length = i;

  /* if there is a file containing nothing */
  if(pf->length == 0)
    return 0;

  /* allocate only enough memory as needed for text alone */
  text = (char*)malloc(i * sizeof(char));

  /* copy the input text to an appropriately sized array */
  for(i=0; input[i] != '\0'; i++)
      text[i] = input[i];
  
  free(input);
  
  pf->text = text;

  return pf;
}

repos read_repo_names(FILE* fp, long NAME_BUF_SIZE)
{
  char* buf = (char*)malloc(256 * sizeof(char));
  char** all_names;
  repos r;
  long index = 0;
  
  /* allocate mem for id struct and all names to be read */
  r = (repos)malloc(sizeof(struct repos));
  all_names = (char**)malloc(NAME_BUF_SIZE * sizeof(char*));

  /* read each name from the given file */
  while(fscanf(fp, "%s", buf) != EOF)
    {
      all_names[index++] = buf;
      buf = (char*)malloc(256 * sizeof(char));
    }

  r->num_names = index;
  r->names = all_names;

  return r;
}


char* create_repo_name_file(char* PATH)
{
  char* repo_fn;
  
  /* array that will hold the command we want the shell to execute */
  char *command = malloc((strlen(PATH)+1024)*sizeof(char));
  
  /* create a temp file */
  repo_fn = mktmp();
  /* create the command string that the shell will execute */
  sprintf(command, "for dir in `ls -d %s/*/`; do " 
	  "basename $dir; done >%s", PATH, repo_fn);

  /* execute the command that will create a temp file containing repo names */
  system(command);

  free(command);

  return repo_fn;
}
