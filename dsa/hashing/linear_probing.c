#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <assert.h>

int size = 10;
int *arr, *temp_arr;

int hash_func(int, int);

int main(void) {
	int data, key;
	uint8_t choice;
	arr = (int *) malloc(sizeof(int)*size);
	for(int i = 0; i < size; i++) {
		*(arr+i) = INT_MIN;
	}
	while(1) {
		printf("Choose:\n1. Insert\n2. Delete\n3. Search\n4. Exit\nEnter you choice: ");
		scanf(" %hhu", &choice);
		if(choice == 1 || choice == 2 || choice == 3) {
			printf("Enter the data value: ");
			scanf(" %d", &data);
		}
		switch(choice) {
			case 1:
			{
				key = hash_func(data, 0);
				if(key == -1) {
					size *= 2;
					temp_arr = (int *) malloc(sizeof(int)*size);
					for(int i = 0; i < size; i++) {
						arr[i] = INT_MIN;
					}
					for(int i = 0; i < size/2; i++) {
						key = hash_func(arr[i], 0);
						temp_arr[key] = arr[i];
					}
					int *to_free = arr;
					arr = temp_arr;
					free(to_free);
					key = hash_func(data, 0);
					assert(key < size);
					arr[key] = data;
				}
				else {
					arr[key] = data;
				}
				puts("\n\nInserted.\n\n");
				break;
			}
		}
	}
	return 0;
}

int hash_func(int value, int current) {
	if(current == size)
		return -1;
	int key = value % size;
	if(arr[key] == INT_MIN) {
		return key;
	}
	else {
		return hash_func(key+1, current+1);
	}
}
