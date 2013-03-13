#ifndef __PARSER_H__
#define __PARSER_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structs.h"
#include"mktmp.h"

/* return the parsed file containing text and length of text */
parsed_file parse(FILE* fp, int WHITESPACE, int PARSE_ALL);

/* read the repo names from a file */
repos read_repo_names(FILE* fp, long NAME_BUF_SIZE);

/* create a temporary file with the names of all repositories */
char* create_repo_name_file(char* PATH);

#endif
