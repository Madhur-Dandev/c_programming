#include <stdio.h>
#include <stdlib.h>

typedef struct node node;

struct node {
	int data;
	node *next;
};

int node_count;
node *head;
node *tracker;

node *create_node(int);
int append(int);
int prepend(int);
int insert(int, int);
int pop(void);
int del_frnt(void);
int delete(int);
void display(void);
void rev_dspy(void);
void rev_dspy_main(node *);

int main(void) {
	append(1);
	append(2);
	printf("%d %d\n", head->data, tracker->data);
	return 0;
}

node *create_node(int data) {
	node *nn = (node *) calloc(1, sizeof(node));
	if(nn == NULL) {
		puts("Memory Error. Unable to add data");
		return nn;
	}
	nn->data = data;
	nn->next = nn;
	return nn;
}

int append(int data) {
	node *nn = create_node(data);
	if(!nn)
		return 0;

	node_count++;

	if(head == NULL) {
		head = tracker = nn;
		goto success;
	} else {
		tracker->next = nn;
		nn->next = head;
		tracker = nn;
	}

	success: return 1;
}

int prepend(int data) {
	node *nn = create_node(data);
	if(!nn)
		return 0;

	node_count++;

	if(head == NULL) {
		head = tracker = nn;
		goto success;
	} else {
		nn->next = head;
		tracker->next = nn;
		head = nn;
	}

	success: return 1;
}

int insert(int data, int pos) {
	if(pos < 1 || pos > node_count) {
		puts("Invalid Location");
		return 0;
	}

	node *temp = head, *main;
	for(int i = 0; i < pos-2; i++) {
		temp = temp->next;
	}

	main = temp->next;
	temp->next = temp->next->next;
	free(main);
	puts("Value Deleted");
	node_count--;

	if(node_count == 0)
		head = tracker = NULL;

	return 1;
}

int pop(void) {
	if(node_count <= 0) {
		puts("List is empty");
		return 0;
	}

	node *temp = head, *to_del = tracker;
	if(node_count == 1) {
		head = tracker = NULL;
		goto to_delete;
	}

	for(int i = 0; i < node_count-2; i++) {
		temp = temp->next;
	}

	temp->next = head;

	to_delete:
		free(to_del);
		node_count--;
		return 1;
}

int del_frnt(void) {
	if(node_count <= 0) {
		puts("List is empty");
		return 0;
	}

	node *to_del = head;
	if(node_count == 1) {
		head = tracker = NULL;
		goto finalize;
	}

	head = head->next;
	tracker->next = head;

	finalize:
		free(to_del);
		node_count--;
		return 1;
}

int delete(int pos) {
	if(pos < 1 || pos > node_count) {
		puts("Invalid Location");
		return 0;
	}

	if(pos == 1 || pos == node_count) {
		pos == 1 ? del_frnt() : pop();
		goto success;
	}

	node *temp = head, *to_del;
	for(int i = 0; i < pos-2; i++) {
		temp = temp->next;
	}

	to_del = temp->next;
	temp->next = temp->next->next;
	free(to_del);

	success:
		return 1;
}

void display(void) {
	node *temp = head;

	do{
		printf("%d ", temp->data);
	}while((temp = temp->next));
	putc('\n', stdout);
}

void rev_dspy(void) {
//	node *temp = tracker;
//	char main[2147483647], buff[2147483647];
//	int curr_idx = 0;
//
//	do{
////		printf("%d ", temp->data);
//		buff =
//	}while((temp = temp->next));
//	putc('\n', stdout);
	node *temp = head;
	rev_dspy_main(temp);
}

void rev_dspy_main(node *data_node) {
	if(data_node) {
		printf("%d ", data_node->data);
		rev_dspy_main(data_node->next);
	} else {
		putc('\n', stdout);
		return;
	}
}
