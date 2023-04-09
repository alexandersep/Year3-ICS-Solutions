#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// a linked list node containing a string
struct listnode {
  char * str;
  struct listnode * next;
};

// a linked list data structure
struct linkedlist {
  struct listnode * head;
};

// create a new, empty linked list set
struct linkedlist * linkedlist_new();

/* check to see if an item is in the set
   returns 1 if in the set, 0 if not */
int linkedlist_lookup(struct linkedlist * this, char * item);

// add an item, with number 'item' to the set
// has no effect if the item is already in the set.
// New items that are not already in the set should
// be added to the start of the list
void linkedlist_add(struct linkedlist * this, char * item);

// remove an item with number 'item' from the set
void linkedlist_remove(struct linkedlist * this, char * item);

// free the linked list structure
void linkedlist_free(struct linkedlist * this);

#endif
