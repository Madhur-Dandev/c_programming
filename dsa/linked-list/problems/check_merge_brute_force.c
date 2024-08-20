/*
 * This is a brute force method
 * Time Complexity O(mn)
 */

#define FORWARD_ONLY 1
#include "headers.h"
#include <inttypes.h>

int main(void) {
	fo_node *head, *temp, *temp2;
	temp = head = (fo_node *) malloc(sizeof(fo_node));
	head->data = 1;

	for(int i = 0; i < 10; i++) {
		temp2 = (fo_node *) malloc(sizeof(fo_node));
		temp2->data = i+2;
		temp->next = temp2;
		temp = temp2;
	}

	fo_node *head2;

	temp = temp2 = NULL;
	temp = head2 = (fo_node *) malloc(sizeof(fo_node));
	head2->data = 1;

	for(int i = 0; i < 3; i++) {
		temp2 = (fo_node *) malloc(sizeof(fo_node));
		temp2->data = i+2;
		temp->next = temp2;
		temp = temp2;
	}

	temp->next = head->next->next->next->next->next;

	temp = head;

	while(temp != NULL) {
		temp2 = head2;
		uint8_t same = 0;

		while(temp2 != NULL) {
			if(temp == temp2) {
				same = 1;
				break;
			}
			temp2 = temp2->next;
		}

		if(same == 1)
			break;

		temp = temp->next;
	}

	printf("%d\n", temp2->data);
	return 0;
}
