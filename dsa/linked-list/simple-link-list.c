#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node node;
struct node {
	int data;
	node* nextNode;
};

node *head = NULL;
node *tracker = NULL;
int nodeCount = 0;
node* createnode(int);
void addnode();
void insert();
void print();
void delete();
void reverse();
void printreverse(node *);
void recurrev(node *, node *);
void clear();

int main(void) {
	bool flag = true;
	while(flag) {
		int choice;
		printf("What you want to do:\n1.\tAdd\n2.\tDelete\n3.\tInsert\n4.\tPrint\n5.\tReverse\n6.\tClear\n7.\tExit\nEnter: ");
		scanf("%d", &choice);
		switch(choice) {
			case 1:
				addnode();
				break;
			case 2:
				delete();
				break;
			case 3:
				insert();
				break;
			case 4:
				print();
				break;
			case 5:
				reverse();
				break;
			case 6:
				clear();
				break;
			case 7:
				flag = false;
				break;
			default:
				printf("Invalid opeartion\n");
		}
	}
	return 0;
}

node* createnode(int data) {
	node* nn = (node *)malloc(sizeof(node));
	nn->data = data;
	return nn;
}

void addnode() {
	int data;
	printf("Enter the value to store: ");
	scanf("%d", &data);
	node *nn = createnode(data);
	if(head == NULL) {
		head = nn;
		tracker = nn;
	}
	else {
		tracker->nextNode = nn;
		tracker = nn;
	}
	nodeCount++;
}

void insert() {
	int data, index;
	printf("Enter the value to store ");
	scanf("%d", &data);
	printf("at location: ");
	scanf("%d", &index);
	if(index > nodeCount) {
		printf("Index out of bound.");
		return;
	}

	node *nn = createnode(data);
	nodeCount++;

	if(index == 1) {
		nn->nextNode = head->nextNode;
		head = nn;
		return;
	}

	node *temp = head;
	for(int i = 0; i < index-2; i++) {
		temp = temp->nextNode;
	}
	nn->nextNode = temp->nextNode;
	temp->nextNode = nn;
}

void print() {
	node *temp = head;
	while(temp != NULL) {
		printf("%d ", temp->data);
		temp = temp->nextNode;
	}
	printf("\n");
}

void delete() {
	int index;
	printf("Enter the value to store: ");
	scanf("%d", &index);
	if(head == NULL) {
		printf("Invalid index. List is empty.\n");
		return;
	}
	if(index > nodeCount) {
		printf("Invalid index. Index out of bound.\n");
		return;
	}

	if(index == 1) {
		node * temp = head;
		head = head->nextNode;
		free(temp);
		return;
	}

	node *temp1 = head;
	for(int i = 0; i < index-2; i++) {
		temp1 = temp1->nextNode;
	}

	node *temp2 = temp1->nextNode;
	temp1->nextNode = temp1->nextNode->nextNode;
	free(temp2);
	printf("Changed list: ");
	print();
}

void reverse() {
	node *temp1 = head;
	node *temp2 = NULL;
	node *temp3 = NULL;

	while(temp1 != NULL) {
		temp3 = temp1->nextNode;
		temp1->nextNode = temp2;
		temp2 = temp1;
		temp1 = temp3;
	}

	head = temp2;
	print();
}

void printreverse(node *temp) {
	if(temp == NULL) return;
	printreverse(temp->nextNode);
	printf("%d ", temp->data);
}

void recurrev(node *temp, node *prev) {
	if(temp->nextNode == NULL) {
		temp->nextNode = prev;
		head = temp;
		return;
	}

	recurrev(temp->nextNode, temp);
	temp->nextNode = prev;
}

void clear() {
	return;
}
