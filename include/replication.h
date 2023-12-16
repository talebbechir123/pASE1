
#ifndef __REPLICATION_H__
#define __REPLICATION_H__
#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>






// replication of a hashtable to a text file

void hashtable_replicate(hashtable *ht, char *filename);

// convert a text file to a hashtable

hashtable *hashtable_convert(char *filename);

// write a hashtable as a json file

void hashtable_write_json(hashtable *ht, char *filename);









#endif