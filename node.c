////////////////////////////////////////////////////////////////////////////////
//
// Main File:        537malloc.c
// This File:        node.c
// Semester:         CS 537 Fall 2018
//
// Authors:          Natalie Brooks, Brianna Benson
// Email:            nrbrooks@wisc.edu, bbenson7@wisc.edu
// CS Login:         natalie
//
////////////////////////////////////////////////////////////////////////////////

#include "node.h"
#include <stdio.h>

Node *createNode(void *ptr, size_t size) {
    Node *new_node = malloc(sizeof(Node));
    if(new_node == NULL) {
        fprintf(stderr, "Not enough room in memory for node to be made.");
        exit(0);
    }
    new_node->freed = false;
    new_node->color = RED;
    new_node->size = size;
    new_node->ptr = ptr;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
}

Node *grandparent(Node *node) {
    if ( (node == NULL) || (node->parent == NULL) || (node->parent->parent == NULL) ) {
        return NULL;
    }
    return node->parent->parent;
}

Node *sibling(Node *node) {
    if( (node == NULL) || (node->parent == NULL) ) {
        return NULL;
    }
    if (node == node->parent->right)
        return node->parent->left;
    else
        return node->parent->right;
}

Node *uncle(Node *node) {
    if ( (node == NULL) || (node->parent == NULL) || (node->parent->parent == NULL) ) {
        return NULL;
    }
    return sibling(node->parent);
}

int color(Node *node) {
    return (node == NULL) ? BLACK : node->color;
}

int *compare(Node *focus, Node *compare) {
    return (int)(&(focus->ptr) - &(compare->ptr));
}

/*
 * Returns 1 if the node has two non-null children and
 * 0 if it does not.
 */
int hasTwoChildren(Node *node) {
    if ( node->left != LEAF && node->right != LEAF ) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Returns pointer to the rightmost node of the tree beneath
 * node passed.
 */
Node *findRightMost(Node *node) {
    if ( node->right == LEAF ) {
        return node;
    } else {
        return findRightMost(node->right);
    }
}
