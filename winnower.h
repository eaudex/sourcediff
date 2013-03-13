#ifndef __WINNOWER_H__
#define __WINNOWER_H__

#include<stdio.h>
#include<stdlib.h>
#include"structs.h"

/* select hashes based on winnowing algorithm */
parsed_file winnow(parsed_file pf, int window_size);

#endif
