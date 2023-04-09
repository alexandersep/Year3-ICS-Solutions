#ifndef NODE_H
#define NODE_H 

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// A BST, is a collection of binary nodes with 
// left and right subtrees, where comparisons are 
// used in val and key to store on left/right node
struct node {
    char * str; // string value
    int size; // size of struct
    struct node *left, *right; // recursive call to a left and right node
};

// creates a new empty bst 
struct node * node_new(float val);

// inserts a value into a node at key
struct node * node_insert(struct node * bst, char * str);

// lookup string in bst
// Return: 1 if found
//         0 if not found
bool node_lookup(struct node * bst, char * str);

// Free node structure
void node_free(struct node * bst);

#endif
