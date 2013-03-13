#ifndef __COMPARE_H__
#define __COMPARE_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structs.h"

/* return the percentage of similarity between two files */
double compare_files(parsed_file file1, parsed_file file2);

/* compares all of the files that have been read in */
void compare_all(parsed_file* pfs, repos repo_names, double THRESH);

#endif
