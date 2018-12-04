#ifndef NODE_H_
#define NODE_H_

#include <stdbool.h>
#include <stdlib.h>

#define RED 0
#define BLACK 1
#define RIGHT 1
#define LEFT 0
#define LEAF NULL

/*
 * This is our Node structure which holds our address and length tuple and a
 * boolean for whether or not the address has been freed.  The Node also
 * contains variables for the structure of our red-black tree including pointers
 * for the left and right children, the parent, and the color of the node.
 */
typedef struct Node {
    // true if freed, false if not
    bool freed;
    // should hold either RED or BLACK
    int color;
    // size of allocated memory
    size_t size;
    // address of first byte allocated
    void *ptr;
    // root node for left child's tree
    struct Node *left;
    // root node for right child's tree
    struct Node *right;
    // parent node
    struct Node *parent;
} Node;

/*
 * This method creates a new node by setting the ptr and size variables and
 * setting the remaining variables to NULL.
 */
Node* createNode(void *ptr, size_t size);

Node *grandparent(Node *node);

Node *sibling(Node *node);

Node *uncle(Node *node);

int color(Node *node);

int *compare(Node *focus, Node *compare);

int hasChild(Node *node);

int hasTwoChildren(Node *node);

Node *findFightMost(Node *node);

#endif
