#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stdbool.h>
#include "linkedlist.h"

struct hashtable {
    struct linkedlist ** table;
    int size;
};

// allocate a new, empty hashtable with size
struct hashtable * hashtable_new(int size);

// lookup string in hashtable 
// Return: 1 if found
//         0 if not found 
bool hashtable_lookup(struct hashtable * ht, char * item);

// If not in table add a string to the table
// else do nothing
void hashtable_add(struct hashtable * ht, char * item);

// remove an item from the hash table; if the item is in the table
// multiple times, just remove the first one that we encounter
void hashtable_remove(struct hashtable * ht, char * item);

// free the memory of the hashtable
void hashtable_free(struct hashtable * ht);

#endif
