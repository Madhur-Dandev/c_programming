#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define DEG 10

typedef struct key_node
{
	int value;
	struct keynode *left;
	struct keynode *right;
} Key;

typedef struct node
{
	int size;
	Key *arr[DEG];
} Node;

Node *make_node(int, ...);
void insert(Node **, int);

int main(void)
{
	make_node(5, 1, 2, 3, 4, 5);
	return 0;
}

Node *make_node(int count, ...)
{
	Node *new = (Node *) malloc(sizeof(Node));
	if(new == NULL)
	{
	}
	va_list args;
	va_start(args, count);
	for(int i = 0; i < count; i++)
	{
		printf("%d\n", va_arg(args, int));
	}
	va_end(args);
	return NULL;
}
