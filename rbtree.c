////////////////////////////////////////////////////////////////////////////////
//
// Main File:        537malloc.c
// This File:        rbtree.c
// Semester:         CS 537 Fall 2018
//
// Authors:          Natalie Brooks, Brianna Benson
// Email:            nrbrooks@wisc.edu, bbenson7@wisc.edu
// CS Login:         natalie
//
////////////////////////////////////////////////////////////////////////////////

/*
 * Implementation of Red Black Tree data structure to serve as 
 * a way to store, access, and delete allocated tuples.

 * PUBLICE
 * Accessors: search
 * Mutators: add, remove, createNode, replaceNode
 * 
 * PRIVATE
 * Balance: rotateRight, rotateLeft, changeColor
 * Insert: compare, insertRebalance
 * Delete: deleteRebalance
 */
#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

Tree* createTree() {
	Tree *tree = malloc(sizeof(Tree));
	tree->root = NULL;
	return tree;
}

void add_case1(Tree *tree, Node *node);
void deleteCase1(Tree *tree, Node *node);

void replace(Tree *tree, Node *old_node, Node *new_node) {
    if (old_node->parent == NULL) {
        tree->root = new_node;
    } else {
        if (old_node == old_node->parent->left)
            old_node->parent->left = new_node;
        else
            old_node->parent->right = new_node;
    }
    if (new_node != NULL) {
        new_node->parent = old_node->parent;
    }
}

void rotate_left(Tree *tree, Node *node) {
    Node *right_child = node->right;
    replace(tree, node, right_child);
    node->right = right_child->left;
    if (right_child->left != NULL) {
        right_child->left->parent = node;
    }
    right_child->left = node;
    node->parent = right_child;
}

void rotate_right(Tree *tree, Node *node) {
    Node *left_child = node->left;
    replace(tree, node, left_child);
    node->left = left_child->right;
    if (left_child->right != NULL) {
        left_child->right->parent = node;
    }
    left_child->right = node;
    node->parent = left_child;
}

/*
 * Continuation of case 4.  Changes the color of the parent and grandparent and
 * rotates if needed.
 */
void add_case5(Tree *tree, Node *node) {
    node->parent->color = BLACK;
    grandparent(node)->color = RED;
    if (node == node->parent->left && node->parent == grandparent(node)->left) {
        rotate_right(tree, grandparent(node));
    } else if (node == node->parent->right && node->parent == grandparent(node)->right) {
        rotate_left(tree, grandparent(node));
    } else {
        fprintf(stderr, "Error building tree.");
        exit(0);
    }
}

/*
 * Balances the tree in the case where rotation is neccessary.
 */
void add_case4(Tree *tree, Node *node) {
    if (node == node->parent->right && node->parent == grandparent(node)->left) {
        rotate_left(tree, node->parent);
        node = node->left;
    } else if (node == node->parent->left && node->parent == grandparent(node)->right) {
        rotate_right(tree, node->parent);
        node = node->right;
    }
    add_case5(tree, node);
}

/*
 * Balances the tree by chancing the color of the parent's generation and
 * onwards.
 */
void add_case3(Tree *tree, Node *node) {
    if(color(uncle(node)) == RED) {
        node->parent->color = BLACK;
        uncle(node)->color = BLACK;
        grandparent(node)->color = RED;
        add_case1(tree, grandparent(node));
    } else {
        add_case4(tree, node);
    }
}

/*
 * If the parent's color is black we're done.
 */
void add_case2(Tree *tree, Node *node) {
    if (color(node->parent) == BLACK)
        return;
    else
        add_case3(tree, node);
}

/*
 * If the node is the root, set color to black.
 */
void add_case1(Tree *tree, Node *node) {
    if (node->parent == NULL)
        node->color = BLACK;
    else
        add_case2(tree, node);
}

/*
 * Inserts a node into the tree.
 */
void add(Tree *tree, Node *new_node) {	
	if(tree->root == NULL) {
		tree->root = new_node;
	}
	else {
		Node *node = tree->root;
		while(1) {
			int temp = compare(new_node, node);
			if (temp == 0) {
				// error new node at same address as root
			} else if (temp < 0) {
				if (node->left == NULL) {
					node->left = new_node;
					break;
				} else {
					node = node->left;
				}
			} else {
				if (node->right == NULL) {
					node->right = new_node;
					break;
				} else {
					node = node->right;
				}
			}
		}
		new_node->parent = node;
	}
	add_case1(tree, new_node);
}

/*
 * case 4 for how to rebalance the tree
 */
void deleteCase4(Tree *tree, Node *node) {
    if ( (node == node->parent->left) &&
        (color(sibling(node)) == BLACK) &&
        (color(sibling(node)->left) == RED) &&
        (color(sibling(node)->right) == BLACK) ) {
        
        sibling(node)->color = RED;
        sibling(node)->left->color = BLACK;
        rotate_right(tree, sibling(node));
        
    } else if ( (node == node->parent->right) &&
               (color(sibling(node)) == BLACK) &&
               (color(sibling(node)->right) == RED) &&
               (color(sibling(node)->left) == BLACK) ) {
        
        sibling(node)->color = RED;
        sibling(node)->right->color = BLACK;
        rotate_left(tree, sibling(node));
    }
    sibling(node)->color = color(node->parent);
    node->parent->color = BLACK;
    if ( node == node->parent->left ) {
        sibling(node)->right->color = BLACK;
        rotate_left(tree, node->parent);
    } else {
        sibling(node)->left->color = BLACK;
        rotate_right(tree, node->parent);
    }
}

/*
 * case 3 for how to rebalance the tree
 */
void deleteCase3(Tree *tree, Node *node) {
    if ( (color(node->parent) == RED) &&
        (color(sibling(node)) == BLACK) &&
        (color(sibling(node)->left) == BLACK) &&
        (color(sibling(node)->right) == BLACK) ) {
        
        sibling(node)->color = RED;
        node->parent->color = BLACK;
    } else {
        deleteCase4(tree, node);
    }
}

/*
 * case 2 for how to rebalance the tree
 */
void deleteCase2(Tree *tree, Node *node) {
    if ( (color(node->parent) == BLACK) &&
        (color(sibling(node)) == BLACK) &&
        (color(sibling(node)->left) == BLACK) &&
        (color(sibling(node)->right) == BLACK) ) {
        
        sibling(node)->color = RED;
        deleteCase1(tree, node->parent);
    } else {
        deleteCase3(tree, node);
    }
}

/*
 * case 1 for how to rebalance the tree
 */
void deleteCase1(Tree *tree, Node *node) {
    if ( node->parent == NULL ) {
        return;
    } else {
        if ( color(sibling(node)) == RED ) {
            node->parent->color = RED;
            sibling(node)->color = BLACK;
            if ( node == node->parent->left ) {
                rotate_left(tree, node->parent);
            } else {
                rotate_right(tree, node->parent);
            }
        }
        deleteCase2(tree, node);
    }
}

/*
 * Removes a node from the tree.
 */
void removeNode(Tree *tree, Node *node) {
	Node *child;
	if ( node == LEAF ) {
		// node has already been deleted
		return;
	}
	if ( hasTwoChildren(node) ) {
		Node *replacement = findRightMost(node->left);
		node->freed = replacement->freed;
		node->size = replacement->size;
		node->ptr = replacement->ptr;
		node = replacement;
	}
	child = (node->right == LEAF) ? node->left : node->right;
	if ( color(node) == BLACK ) {
		node->color = color(child);
		deleteCase1(tree, node);
	}
	replace(tree, node, child);
	if ( node->parent == NULL && child != LEAF ) {
		child->color = BLACK;
	}
	free(node);
}

/*
 * Searches the tree and returns an array of nodes who are included
 * in the range provided.
 */
void getOverlapNodes(Node *node, Node **nodes, int k, int start_addr, int end_addr) {
    if ( node == NULL ) {
        return;
    }
    
    int start_addr = &(node->ptr);
    int end_addr = start_addr + node->size;
    
    if ( inRange(node, start_addr, end_addr) ) {
        nodes[k] = node;
        return getOverlapNodes(node->right, nodes, k + 1, start_addr, end_addr);
        return getOverlapNodes(node->lefft, nodes, k + 1, start_addr, end_addr);
    } else {
        if ( greaterThanRange(node, end_addr) ) {
            return getOverlapNodes(node->left, nodes, k, start_addr, end_addr);
        } else {
            return getOverlapNodes(node->right, nodes, k, start_addr, end_addr)
        }
    }
}

/*
 * Returns true if the node starts after the end_addr.
 */
bool *greaterThanRange(Node *node, int end_addr) {
    int node_start = &(node->ptr);
    if ( node_start > end_addr ) {
        return true;
    } else {
        return false;
    }
}

/*
 * Determines if a node overlaps the range between start_addr and
 * end_addr. Returns true if it does, false if it does not.
 */
bool *inRange(Node *node, int start_addr, int end_addr) {
    if ( node == NULL ) {
        return false;
    } else {
        int node_start = &(node->ptr);
        int node_end = node_start + node->size;
        // node starts within bounds
        if ( node_start >= start_addr && node_start <= end_addr ) {
            return true;
            // node ends within bounds
        } else if ( node_end >= start_addr && node_end <= end_addr ) {
            return true;
            // range is contained within the node
        } else if ( node_start <= start_addr && node_end >= end_addr ) {
            return true;
        } else {
            return false;
        }
    }
}

/*
 * Searches the tree and returns a node whose range includes
 * the address pointed to by ptr. Returns NULL if there are
 * none.
 */
Node *rangeSearch(Node *node, void *ptr) {
    int start_addr = &(node->ptr);
    int end_addr = start_addr + node->size;
    
    if ( node == NULL ) {
        return NULL;
    } else {
        if ( start_addr <= &(ptr) && end_addr >= &(ptr) ) {
            return node;
        } else if ( &(node->ptr) <= &(ptr) ) {
            return rangeSearch(node->right, ptr);
        } else {
            return rangeSearch(node->left, ptr);
        }
    }
}

