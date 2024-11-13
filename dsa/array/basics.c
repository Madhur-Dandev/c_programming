#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// traversal
void traversal(int *, int);
void deletion(int *, int, int);
void insertion(int *, int, int, int); 

int main() {
	int arr[5] = {1, 2, 3, 4, 5};
	traversal(arr, 5);
	deletion(arr, 5, 3);	
	traversal(arr, 5);
	insertion(arr, 3, 5, 10);
	traversal(arr, 5);
	return 0;
}

void traversal(int *arr, int size) {
	for(int i = 0; i < size; i++) {
		printf("%d\n", *(arr+i));
	}

	return;
}

void deletion(int *arr, int size, int position) {
	if(position > size || position < 0) {
		printf("Invalid position\n");
		return;
	}
	arr[position - 1] = INT_MIN;
	return;
}

void insertion(int *arr, int position, int size, int val) {
	// this is just value altering opeartion
	if(position > size || position < 0) {
		printf("Invalid position\n");
		return;
	}
	arr[position - 1] = val;
	return;
}
