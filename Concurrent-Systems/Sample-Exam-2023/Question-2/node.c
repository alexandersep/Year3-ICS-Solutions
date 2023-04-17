#include "node.h"

/* sources: https://www.programiz.com/dsa/binary-search-tree,
            https://stackoverflow.com/questions/67782513/converting-a-bst-to-an-array,
            https://algs4.cs.princeton.edu/32bst/ 
            https://stackoverflow.com/questions/29582431/convert-binary-tree-to-array-in-c 
*/

// create a new empty bst 
struct node * node_new() {
    struct node * single_node = (struct node *)malloc(sizeof(struct node)); // memory for node struct
    single_node->str = NULL; // assign value to node
    single_node->left = NULL; // single node has no left node
    single_node->right = NULL; // single node has no right node
    single_node->size = 0; // single node is size 1
    return single_node; // reutrn newley created node
}

// inserts a string into a node at key
struct node * node_insert(struct node * bst, char* str) {
    // base case, if empty  
    if (bst == NULL) { // if empty node
        struct node * bst = node_new(); // insert a new node with string and key
    }
    bst->size += 1; // increment size counter of struct as we are inserting
    if (bst->str > str)
        bst->left = node_insert(bst->left,str); // insert string in left side of the tree
    else
        bst->right = node_insert(bst->right,str); // insert string in right side of tree
    return bst; // return newly inserted string
}

// lookup string in bst
// Return: 1 if found
//         0 if not found
bool node_lookup(struct node * bst, char * str) {
    if (strcmp(bst->str, str) == 0) {
        return true;        
    } else if (bst == NULL) {
        return false;
    }
    if (strcmp(bst->str, str) > 0) {
        bool is_found = node_lookup(bst->left, str);
    } 
    return node_lookup(bst->right, str);
}

// free() node structure
void node_free(struct node * bst) {
    if (bst == NULL) // if bst is empty 
        return; // return since node is empty
    // if bst is not empty traverse left side recursively
    node_free(bst->left);
    // if bst is not empty traverse right side recursively
    node_free(bst->right);
    free(bst);
}
