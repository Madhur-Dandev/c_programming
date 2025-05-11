#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	Node **list = init();
	add_connection(list, 1, 2);
	delete_connection(list, 1, 2);
	freeup(list);
	return 0;
}

Node **init()
{
	Node **list = (Node **) malloc(sizeof(Node *) * VRTCS);
	if(list == NULL)
	{
		perror("Failed to add the edge");
		return NULL;
	}
	return list;
}

void add_connection(Node **list, int vrtx, int lk_vrtx)
{
	// vrtx -> vertex
	// lk_vrtx-> link vertex

	if(vrtx >= VRTCS)
	{
		fprintf(stderr, "Error: No such vertex");
		return;
	}
	
	Node *new = (Node *) malloc(sizeof(Node));
	if(new == NULL)
	{
		perror("Failed to add the edge");
		return;
	}
	new->value = lk_vrtx;
	new->next = list[vrtx];
	list[vrtx]	= new;
}

void delete_connection(Node **list, int vrtx, int lk_vrtx)
{
	// vrtx -> vertex
	// lk_vrtx-> link vertex

	if(vrtx >= VRTCS)
	{
		fprintf(stderr, "Error: No such vertex");
		return;
	}

	if(list[vrtx] == NULL)
	{
		fprintf(stderr, "No edge from %d to %d\n", vrtx, lk_vrtx);
	}

	Node *curr = list[vrtx - 1];
	Node *prev = NULL;
	while(curr != NULL)
	{
		if(curr->value == lk_vrtx)
		{
			if(prev == NULL)
				list[vrtx] = curr->next;
			else
				prev->next = curr->next;

			break;
		}
		prev = curr;
		curr = curr->next;
	}
	prev = curr;
	free(curr);
}
void freeup(Node **list)
{
	Node *curr, *to_free;

	for(int i = 0; i < VRTCS; i++)
	{
		if(list[i] != NULL)
		{
			curr = list[i]->next;
			while(curr != NULL)
			{
				to_free = curr;
				curr = curr->next;
				free(to_free);
			}
		}
	}

	free(list);
	return;
}
