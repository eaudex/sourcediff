#ifndef __HASH_H__
#define __HASH_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structs.h"
#include"parser.h"
#include"select.h"
#include"mktmp.h"

/* hash a file based on desired substring length to be analyzed */
parsed_file hash_text(parsed_file fp, int ss_lenght);

/* hash all of the repos and return them in an array for comparison */
parsed_file* hash_repos(repos repo_names, char* PATH, char* filename,
			int BUF_SIZE, int WHITESPACE, int SORT, 
			int MOD_SELECTOR, int SS_LENGTH, int PARSE_ALL);

#endif
