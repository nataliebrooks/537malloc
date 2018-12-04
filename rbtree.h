#ifndef _RBTREE_H_
#define _RBTREE_H_

#include "node.h"

/*
 * This method adds a new node to the tree given the root node and the node
 * to be added.
 */
void add(Node *root, Node *new_node);

/*
 * This removes a node from the tree given an address.  The method will first
 * search for the node containing the address, and remove the node if it exits.
 * Otherwise, it will give an error.
 */
void removeNode(Node *root, Node *node);

/*
 * This method with replace a node with another.
 */
void replace(Node *root, Node *old_node, Node *new_node);

/*
 * This method will search throught the tree given the root node and stop when
 * it finds the node containing the given address.
 */
Node* search(Node *root, void *ptr);

/*
 * Searches the tree and returns an array of nodes who are included 
 * in the range provided.
 */
void getOverlapNodes(Node *node, Node **nodes, int k, int start_addr, int end_addr);

/*
 * Returns true if the node starts after the end_addr.
 */
bool *greaterThanRange(Node *node, int end_addr);

/*
 * Determines if a node overlaps the range between start_addr and
 * end_addr. Returns true if it does, false if it does not.
 */
bool *inRange(Node *node, int start_addr, int end_addr);

/*
 * Searches the tree and returns a node whose range includes 
 * the address pointed to by ptr. Returns NULL if there are
 * none.
 */
Node *rangeSearch(void *ptr);

#endif
