#include"mktmp.h"

/* create a temporary file */
char* mktmp()
{
  /* tmpnam expects an input string of len L_tmpnam so we give it a */
  /* meaningless array to set up the temp file then overright it */
  /* with the system call to 'ls' */
  char nothing[L_tmpnam];
  
  /* create the temporary file that we will use with system calls */
  /* currently holds meaningless data that will be overwritten */
  /* using strdup here becuase there were errors where the file name string */
  /* was altered which strdup fixed */
  char* temp_file = strdup(tmpnam(nothing));

  return temp_file;
}
