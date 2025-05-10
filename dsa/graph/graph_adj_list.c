#include <stdio.h>
#include <stdlib.h>

#define VRTCS 4

typedef struct adj_list_node
{
	int value;
	struct adj_list_node *next;
} Node;

Node **init(void);
void add_connection(Node **, int, int);
void delete_connection(Node **, int, int);
void freeup(Node **);

int main(void)
{
	Node **arr = init();
	add_connection(arr, 1, 2);
	delete_connection(arr, 1, 2);
	freeup(arr);
	return 0;
}

Node **init()
{
	Node **arr = (Node **) malloc(sizeof(Node *) * VRTCS);
	Node *new = NULL;
	for(int i = 0; i < VRTCS; i++)
	{
		puts("here");
		new = (Node *) malloc(sizeof(Node));
		new->value = i + 1;
		new->next = NULL;
		arr[i] = new;
	}
	return arr;
}

void add_connection(Node **arr, int vrtx, int lk_vrtx)
{
	// vrtx -> vertex
	// lk_vrtx-> link vertex

	if(vrtx >= VRTCS)
	{
		fprintf(stderr, "Error: No such vertex");
		return;
	}

	Node *head = arr[vrtx - 1];
	while(head->next != NULL)
		head = head->next;
	
	Node *new = (Node *) malloc(sizeof(Node));
	new->value = lk_vrtx;
	new->next = NULL;
	
	head->next = new;
}

void delete_connection(Node **arr, int vrtx, int lk_vrtx)
{
	// vrtx -> vertex
	// lk_vrtx-> link vertex

	if(vrtx >= VRTCS)
	{
		fprintf(stderr, "Error: No such vertex");
		return;
	}

	Node *curr = arr[vrtx - 1]->next;
	Node *prev = arr[vrtx-1];
	while(curr != NULL)
	{
		if(curr->value == lk_vrtx)
		{
			curr->next = curr->next;
			break;
		}
		free(curr);
	}
}
void freeup(Node *arr[])
{
	Node *head, *to_free;

	for(int i = 0; i < VRTCS; i++)
	{
		head = arr[i];
			
		while(head->next != NULL)
		{
			to_free = head;
			head = head->next;
			free(to_free);
		}

		free(head);
	}

	free(arr);
}
