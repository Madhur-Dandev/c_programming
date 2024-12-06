#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>

typedef struct stack stack;

struct stack {
	int top;
	int size;
	int capacity;
	int *arr;
	void (*push) (stack *, int);
	int (*pop) (stack *);
	int (*peek) (stack *);
	bool (*isEmpty)(stack *);
	void (*deleteStack) (stack *);
};

void push(stack *, int);
int pop(stack *);
int peek(stack *);
bool isEmpty(stack *);
void deleteStack(stack *);
stack *makeStack(void);
#endif
