#include "stack.h"

void push(stack *s, int val)
{
	if (s->top >= s->max_capacity)
	{
		// Dynamically resize stack when full.
		int *temp = realloc(s->elements, sizeof(int) * (s->max_capacity * 2));
		if (temp == NULL)
		{
			printf("Stack overflowed\n");
			return;
		}
		s->elements = temp;
		s->max_capacity *= 2;
	}
	s->elements[s->top++] = val;
	return;
}

int pop(stack *s)
{
	if (s->top <= 0)
	{
		printf("Stack is empty.\n");
		return INT_MIN; // Error code for empty stack.
	}

	int val = s->elements[--s->top];
	return val;
}

int peek(stack *s)
{
	if (s->top <= 0)
	{
		printf("Stack is empty\n");
		return INT_MIN; // Error code for empty stack.
	}
	return s->elements[s->top - 1];
}

bool is_empty(stack *s)
{
	return s->top <= 0;
}

void delete_stack(stack *s)
{
	if (s->elements == NULL)
	{
		return;
	}

	free(s->elements);
	s->top = 0;
	free(s);
	printf("Stack deleted\n");
	return;
}

stack *make_stack()
{
	stack *s = malloc(sizeof(stack));
	if (s == NULL)
	{
		printf("Unable to make stack\n\tMemory allocation failed\n");
		return NULL;
	}
	s->top = 0;
	s->max_capacity = 8;
	s->elements = (int *)malloc(sizeof(int) * 8);
	if (s == NULL)
	{
		printf("Unable to make stack\n\tMemory allocation failed\n");
		return NULL;
	}

	// Assign function pointers
	s->push = &push;
	s->pop = &pop;
	s->peek = &peek;
	s->is_empty = &is_empty;
	s->delete_stack = &delete_stack;
	return s;
}
