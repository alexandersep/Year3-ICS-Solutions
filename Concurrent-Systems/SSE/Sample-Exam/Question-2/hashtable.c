// Modified Hashtable from David Gregg, November 2020
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashtable.h"

// Hash function created by Dan Bernstein, known as djb2
// Source: http://www.cse.yorku.ca/~oz/hash.html
// compute a hash of a string using a seed value, where the result
// falls between zero and range-1
int hash_string(char * string, int range) {
  unsigned int hash = 5831;
  int c;
  // simple loop for mixing the input string
  while (c = *string++) {
    hash = ((hash << 5) + hash) + c;
  }
  // check for unlikely case that hash is negative
  return hash % range;
}

// allocate a new, empty hashtable with size
struct hashtable * hashtable_new(int size) {
    struct hashtable * new_table = malloc(sizeof(struct hashtable));
    new_table->size = size;
    new_table->table = malloc(sizeof(new_table->table) * size);
    for (int i = 0; i < size; i++) {
        new_table->table[i] = linkedlist_new(); 
    }
    return new_table;
}

// lookup string in hashtable 
// Return: 1 if found
//         0 if not found 
bool hashtable_lookup(struct hashtable * this, char * item) {
    return linkedlist_lookup(this->table[hash_string(item, this->size)], item);
}

// If not in table add a string to the table
// else do nothing
void hashtable_add(struct hashtable * this, char * item) {
    linkedlist_add(this->table[hash_string(item, this->size)], item);
}

// remove an item from the hash table; if the item is in the table
// multiple times, just remove the first one that we encounter
void hashtable_remove(struct hashtable * this, char * item) {
    linkedlist_remove(this->table[hash_string(item, this->size)], item);
}

// free the memory of the hashtable
void hashtable_free(struct hashtable * this) {
    for (int i = 0; i < this->size; i++) {
        linkedlist_free(this->table[i]); 
    }
    free(this->table);
    free(this);
}
