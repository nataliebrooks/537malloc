#include "537malloc.h"

int main() {
	int *ptr = malloc537(sizeof(int));

	free537(ptr);

	realloc537(ptr, sizeof(int));
}
