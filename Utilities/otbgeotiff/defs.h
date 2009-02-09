
/*
 * This file is included by the CSV ".c" files in the csv directory.
 */

#include <stdio.h>

/* Data structures */
typedef const char * datafile_rows_t;
typedef struct datafile_s {
  const char *name;
  const datafile_rows_t **rows; 
} datafile_t;
