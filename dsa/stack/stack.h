#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>

typedef struct stack stack;

// define stack structure
struct stack
{
	int top;		  // Index of top element
	int max_capacity; // Maximum capacity of the stack
	int *elements;	  // Pointer to stack elements
	void (*push)(stack *, int);
	int (*pop)(stack *);
	int (*peek)(stack *);
	bool (*is_empty)(stack *);
	void (*delete_stack)(stack *);
};

void push(stack *, int);
int pop(stack *);
int peek(stack *);
bool is_empty(stack *);
void delete_stack(stack *);
stack *make_stack(void);
#endif
