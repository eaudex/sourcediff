#ifndef __STRUCTS_H__
#define __STRUCTS_H__

/* data struct created for each file */
struct parsed_file {
  char* repo_name;
  char* text;
  long length;
  long long* hashes;
  long num_hashes;
  long long* chosen_hashes;
  long num_chosen_hashes;
};

/* data struct created to hold all names of student */
struct repos {
  char** names;
  long num_names;
};

typedef struct parsed_file* parsed_file;

typedef struct repos* repos;

#endif
