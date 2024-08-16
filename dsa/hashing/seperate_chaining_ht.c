#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#define MAX_SIZE 10

typedef struct node {
	int data;
	struct node *next;
} node;

node *arr[MAX_SIZE];

int hash_func(int);

int main(void) {
	for(int i = 0; i < MAX_SIZE; i++)
		arr[i] = NULL;

	uint8_t choice;
	node *addr;
	int data;
	int idx;
	while(1) {
		printf("Select:\n1. Insert\n2. Delete\n3. Search\n4. Exit\nEnter your choice: ");
		scanf(" %hhu", &choice);
		if(choice == 1 || choice == 2 || choice == 3) {
			printf("Enter the value to enter");
			scanf(" %d", &data);
		}
		switch(choice) {
			case 1:
				{
					idx = hash_func(data);
					addr = (node *) malloc(sizeof(node));
					addr->data = data;
					if(arr[idx] == NULL) {
						arr[idx] = addr;
						goto done_insert;
					}

					node *temp = arr[idx];
					int counter = 0;
					while(1){
						counter++;
						if(!temp->next)
							break;
						temp = temp->next;
					}

					if(counter >= 10) {
						puts("\n\nNo space available!\n\n");
						free(addr);
						break;
					}

					temp->next = addr;

					done_insert:
						puts("\n\nInserted.\n\n");
						break;
				}
			case 2:
				{
					idx = hash_func(data);
					if(arr[idx] == NULL) {
						puts("\n\nNo such key!\n\n");
						break;
					}

					node *temp, *to_free;
					temp = to_free = arr[idx];
					if(temp->next == NULL) {
						arr[idx] = NULL;
						goto done_del;
					}

					if(temp->next != NULL && temp->data == data) {
						arr[idx] = temp->next;
						goto done_del;
					}

					while(1){
						if(temp->next->data == data)
							break;
						temp = temp->next;
					}

					to_free = temp->next;
					temp->next = temp->next->next;
					done_del:
						free(to_free);
						puts("\n\nDeleted.\n\n");
						break;
				}
			case 3:
				idx = hash_func(data);
				addr = arr[idx];
				int counter = 0;

				if(addr == NULL) {
					not_found:
						puts("\n\nItem not found!\n\n");
						break;
				}

				while(1) {
					counter++;
					if(addr->data == data) {
						goto found;
					}
				}

				goto not_found;

				found:
					printf("\n\nItem found at index %d, position %d\n\n", idx, counter);
					break;
			case 4:
				goto done;
			default:
				puts("Choose a correct option!");
		}
	}

	done:
		return 0;
}

int hash_func(int key) {
	return key % MAX_SIZE;
}
