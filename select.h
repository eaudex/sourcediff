#ifndef __SELECT_H__
#define __SELECT_H__

#include<stdio.h>
#include<stdlib.h>
#include"structs.h"

/* selects only certain hashes based on modular technique */
parsed_file select_hashes(parsed_file pf, int mod);

#endif
