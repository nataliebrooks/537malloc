////////////////////////////////////////////////////////////////////////////////
//
// Main File:        537malloc.c
// This File:        537malloc.c
// Semester:         CS 537 Fall 2018
//
// Authors:          Natalie Brooks, Brianna Benson
// Email:            nrbrooks@wisc.edu, bbenson7@wisc.edu
// CS Login:         natalie
//
////////////////////////////////////////////////////////////////////////////////

#include "537malloc.h"

void *malloc537(size_t size) {
	if ( size == 0 ) {
		fprintf(stderr, "WARNING: Allocating for size 0.");
	} 
	void *ptr = malloc(size);
	Node *node;
	Node **nodes = malloc(sizeof(*Node) * BUFFER);
	int start_addr = &(ptr);
	int end_addr = start_addr + size;
	getOverlapNodes(tree->root, nodes, 0, start_addr, end_addr);
	void *end_ptr = *(ptr + (void*)size);

	if ( nodes[0] == NULL ) {
		node = createNode(ptr, size);
		add(tree->root, node);
	} else {
		int i = 0;
		int head = -1;
		int tail = -1;
		while ( nodes[i] != NULL ) {
			node_start_addr = &node[i]->ptr;
			node_end_addr = node_start_addr + node[i]->size;
			// new pointer is within the range of this node
			if ( addressInNode(node_start_addr, node_end_addr, ptr) ) {
				head = i;
			}
			if ( addressInNode(node_start_addr, node_end_addr, end_ptr) ) {
				tail = i;
			}
			i++;
		}
		if ( head >= 0 ) {
			if ( nodes[head]->ptr == ptr ) {
				nodes[head]->size = size;
				nodes[head]->freed = false;
			} else {
				nodes[head]->size = &ptr - &node[head]->ptr;
				// make new node at ptr with size
				node = createNode(ptr, size);
				node->freed = false;
				add(tree->root, node);
			}
		} else {
			// head does not overlap with any existing node
			node = createNode(ptr, size);
			node->freed = false;
			add(tree->root, node);
		}
		if ( tail >= 0 ) {
			node_start_addr = &node[tail]->ptr;
			node_end_addr = node_start_addr + node[tail]->size;
			if ( node_end_addr == &end_ptr ) {
				// if they overlap perfectly, we can just delete that node
				tail = -1
			} else {
				// make new node for extra
				node_size = node_end_addr - &end_ptr;
				node = createNode(end_ptr, node_size);
				add(tree->root, node);
				tail = -1
			}
		}
		// now remove all nodes that are not the head
		i = 0;
		while ( nodes[i] != NULL ) {
			if ( i != head ) {
				removeNode(tree->root, nodes[i]);
			}
		}
	}
}

bool addressInNode(int start_addr, int end_addr, void *ptr) {
	if ( start_addr <= &ptr && end_addr >= &ptr ) {
		return true;
	} else {
		return false;
	}
}

void free537(void *ptr) {
	Node *node = rangeSearch(tree->root, ptr);
	if ( node == NULL ) {
		fprintf(stderr, "ERROR: Freeing memory that has not been allocated with malloc537().");
		exit(-1);
	} else if ( node->ptr != ptr ) {
		fprintf(stderr, "ERROR: Freeing memory that is not the first byte of the range of memory that was allocated.");
		exit(-1);
	} else if ( node->freed == true ) {
		fprintf(stderr, "ERROR: Freeing memory that has already been freed with malloc537().");
		exit(-1);
	} else {
		free(ptr);
	}
}

void *realloc537(void *ptr, size_t size) {
	if ( ptr == NULL ) {
		malloc537(size);
	} else if ( size == 0 ) {
		free537(ptr);
	} else {
		void *new_ptr = realloc(ptr, size);
		free537(ptr);
		malloc537(new_ptr, size);
	}
}

void memcheck537(void *ptr, size_t size) {
	void *end_ptr = *(ptr + (void*)size);
	Node *node = rangeSearch(tree->root, ptr);
	if ( node == NULL ) {
		fprintf(stderr, "ERROR: Accessing bad memory: Memory has not been allocated by malloc537().");
		exit(-1);
	}
	if ( node->ptr == ptr ) {
		if ( node->freed == false ) {
			if ( node->size >= size ) {
				return;
			} else {
				fprintf(stderr, "ERROR: Accessing bad memory: Memory extends beyond a single block allocated by malloc537().");
				exit(-1);
			}
		} else {
			fprintf(stderr, "ERROR: Accessing bad memory: Memory has been freed by free537().");
			exit(-1);
		}
	} else {
		fprintf(stderr, "ERROR: Accessing bad memory: Memory that does not begin at an address allocated by malloc537().");
		exit(-1);
	}
}


