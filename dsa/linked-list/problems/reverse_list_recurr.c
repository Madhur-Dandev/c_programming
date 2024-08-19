#define FORWARD_ONLY 1
#include "headers.h"

void reverse(fo_node *prev, fo_node *curr, fo_node **head) {
	if(curr == NULL) {
		*head = prev;
		return;
	}
	reverse(curr, curr->next, head);
	curr->next = prev;
	return;
}

int main(void) {
	fo_node *head, *tail, *temp;

	/*
	 * Making a linked list
	 * Starting with a head node
	 * followed by a loop to attach
	 * further more nodes
	 */
	tail = head = (fo_node *) malloc(sizeof(fo_node));
	head->data = 1;

	for(int i = 0; i < 9; i++) {
		temp = (fo_node *) malloc(sizeof(fo_node));
		temp->data = i+2;
		tail->next = temp;
		tail = temp;
	}

	reverse(NULL, head, &head);

	while(temp) {
		printf("%d\n", temp->data);
		temp = temp->next;
	}
}
