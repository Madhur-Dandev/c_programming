#include <stdio.h>
#include <stdlib.h>

typedef struct node node;

struct node {
	int data;
	node *prev;
	node *next;
};

int node_count = 0;
node *head = NULL;
node *tracker = NULL;

node *create_node(int);
void append(int);
void prepend(int);
void insert(int, int);
void front_delete(void);
void last_delete(void);
void delete(int);
void display(void);
void dspy_rev(void);

int main(void) {
	append(1);
	display();
	prepend(0);
	display();
	append(3);
	display();
	insert(2, 3);
	display();
	insert(4, 5);
	display();
	append(4);
	display();
	front_delete();
	display();
	dspy_rev();
	last_delete();
	display();
	dspy_rev();
	delete(1);
	display();
	dspy_rev();
	append(4);
	display();
	dspy_rev();
}

node *create_node(int data) {
	node *nn = (node *) calloc(1, sizeof(node));
	if(nn == NULL) {
		printf("Cannot make new data node\n");
		return nn;
	}
	(*nn).data = data;
	(*nn).prev = NULL;
	(*nn).next = NULL;
	node_count++;
	return nn;
}

void append(int data) {
	node *nn = create_node(data);
	if(nn == NULL) return;
	if(head == NULL) {
		head = nn;
		tracker = nn;
	} else {
		tracker->next = nn;
		nn->prev = tracker;
		tracker = nn;
	}
	printf("New node appended\n");
	return;
}

void prepend(int data) {
	if(head == NULL) {
		append(data);
		return;
	} else {
		node *nn = create_node(data);
		if(nn == NULL) return;
		nn->next = head;
		head->prev = nn;
		head = nn;
	}

	printf("New node prepended\n");
	return;
}

void insert(int data, int pos) {
	if(pos < 1 || pos > node_count) {
		printf("Invalid Position\n");
		return;
	} else {
		if(pos == 1) prepend(data);
		else {
			node *temp = head;
			for(int i = 0; i < pos-1; i++) {
				temp = temp->next;
			}

			node *nn = create_node(data);
			if(nn == NULL) return;
			temp->prev->next = nn;
			nn->next = temp;
			nn->prev = temp->prev;
			temp->prev = nn;
		}
	}

	printf("New node added\n");
	return;
}

void front_delete(void) {
	if(node_count <= 0) {
		printf("Nothing to delete\n");
		return;
	} else {
		node *temp = head;
		temp->next->prev = NULL;
		head = temp->next;
		free(temp);
		node_count--;
		if(node_count == 0) {
			head = NULL;
			tracker = NULL;
		}
	}
	printf("Node Delete\n");
}

void last_delete(void) {
	if(node_count <= 0) {
		printf("Nothing to delete\n");
		return;
	} else {
		node *temp = tracker;
		if(temp->prev)
			temp->prev->next = NULL;
			tracker = temp->prev;

		free(temp);
		node_count--;
		if(node_count == 0) {
			head = NULL;
			tracker = NULL;
		}
	}
	printf("Node Delete\n");
}

void delete(int pos) {
	if(pos < 1 || pos > node_count) {
		printf("Invalid Position\n");
	} else {
		if(pos == 1) {
			front_delete();
			return;
		} else if(pos == node_count) {
			last_delete();
			return;
		} else {
			node *temp = head;
			for(int i = 0; i < pos-1; i++) {
				temp = temp->next;
			}
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			free(temp);
			node_count--;
		}
	}
	printf("Node Delete\n");
}

void display(void) {
	if(node_count <= 0) {
		printf("Nothing to print\n");
		return;
	} else {
		node *temp = head;
		do {
			printf("%d ", temp->data);
		} while((temp = temp->next) != NULL);
		putchar('\n');
	}
}

void dspy_rev(void) {
	if(node_count <= 0) {
		printf("Nothing to print\n");
		return;
	} else {
		node *temp = tracker;
		do {
			printf("%d ", temp->data);
		} while((temp = temp->prev) != NULL);
		putchar('\n');
	}
}
