#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "linkedlist.h"

// create a new, empty linked list set
struct linkedlist * linkedlist_new() {
    struct linkedlist * header = malloc(sizeof(struct linkedlist));
    header->head=NULL;
    return header;
}

/* check to see if an item is in the set
   returns 1 if in the set, 0 if not */
int linkedlist_lookup(struct linkedlist * this, char * item) {
    struct listnode * p;
    for (p = this->head; p != NULL; p = p->next) {
        if (strcmp(item, p->str) == 0) {
            return 1;
        }
    }
    return 0;
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set.
// New items that are not already in the set should
// be added to the start of the list
void linkedlist_add(struct linkedlist * this, char * item) {
    struct listnode * i;
    int is_in_list = 0; // boolean, is item in list, if yes do nothing
    for (i = this->head; i != NULL; i = i->next) {
        if (strcmp(i->str, item) == 0) {
           is_in_list = 1;
           break;
        }
    }
    if (!is_in_list) {
        struct listnode * new_node = malloc(sizeof(struct listnode));
        new_node->next = this->head; // if empty set to null otherwise not
        this->head = new_node;
        new_node->str = item;
    }
}

// remove an item with number 'item' from the set
void linkedlist_remove(struct linkedlist * this, char * item) {
    struct listnode * current;
    struct listnode * prev;

    // is the list empty?
    if (this->head == NULL) {
        return;
    }

    // is the wanted item in first node of the list?
    if (strcmp(this->head->str, item) == 0) {
        current = this->head;
        this->head = current->next;
        free(current);
        return;
    }

    // search remainder of the list for item
    for (prev = this->head, current = this->head->next; current != NULL; prev = current, current = current->next) {
        if (strcmp(current->str, item) == 0) {
            prev->next = current->next;
            free(current);
            return;
        }
    }
}

// free the linked list structure
void linkedlist_free(struct linkedlist * this) {
    struct listnode * current;
    struct listnode * next;
    // search remainder of the list for item
    for (current = this->head; current != NULL; current = next) {
        next = current->next;
        free(current);
    }
    free(this);
}
