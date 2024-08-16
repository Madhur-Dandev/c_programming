#define DOUBLE 1
#include "headers.h"

int main(void) {
#if !defined(DOUBLE)
	printf("Terminated\n");
	exit(1);
#endif

	fb_node *head, *tail;
	head = tail = NULL;
	int counter;

	create_ll(&head, &tail, &counter);
//	printf("%p %p\n", head, tail);

	for(int i = 0; i < 100000000; i++) {
		insert(&tail, i+1, &counter);
	}

//	print_list(&head);

	for(int i = 0; i < 100000000; i++) {
		delete(&head, &tail, &counter);
	}

	print_list(&head);
	destroy_ll(&head, &tail);

	printf("hey\n");
	while(1);
	return 0;
}

void create_ll(fb_node **head, fb_node **tail, int *counter) {
	if(*head || *tail)
		return;

	*head = *tail = (fb_node *) malloc(sizeof(fb_node));

	(*head)->next = (*head)->prev = NULL;

	if(*head == NULL)
		TERMINATE(errno);

	*counter = 0;
	return;
}

void destroy_ll(fb_node **head, fb_node **tail) {
	fb_node *temp;

	while(*head) {
		temp = *head;
		*head = (*head)->next;
		free(temp);
		printf("hey\n");
	}

	*head = *tail = NULL;
}

void insert(fb_node **tail, int val, int *counter) {
	if(*counter == 0) {
		(*tail)->data = val;
		*counter += 1;
		return;
	}

	fb_node *temp = (fb_node *) malloc(sizeof(fb_node));

	if(temp == NULL)
		TERMINATE(errno);

	temp->data = val;
	(*tail)->next = temp;
	temp->prev = *tail;
	*tail = temp;

	*counter += 1;

	return;
}

void delete(fb_node **head, fb_node **tail, int *counter) {
	if(counter <= 0) {
		printf("Stack is empty\n");
		return;
	}

	fb_node *temp = *tail;
	*tail = (*tail)->prev;

	if((*tail) != NULL)
		(*tail)->next = NULL;
	else
		*head = NULL;

	free(temp);

	*counter -= 1;

	return;
}

void print_list(fb_node **head) {
	fb_node *temp = *head;

	while(temp) {
		printf("%d\n", temp->data);
		temp = temp->next;
	}

	return;
}
