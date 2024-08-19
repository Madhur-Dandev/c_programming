#define FORWARD_ONLY 1
#include "headers.h"

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

	for(int i = 0; i < 99; i++) {
		temp = (fo_node *) malloc(sizeof(fo_node));
		temp->data = i+2;
		tail->next = temp;
		tail = temp;
	}

	tail = temp = NULL; /* tail will act as temporary next pointer */
	while(head){
		tail = head->next;
		head->next = temp;
		temp = head;
		head = tail;
	}

	head = temp; /* update the head to last updated value of temp which will hold last node's address */
	temp = head;

	while(temp) {
		printf("%d\n", temp->data);
		temp = temp->next;
	}
}
