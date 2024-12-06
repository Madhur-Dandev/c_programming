#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>

typedef struct stack stack;
typedef int bool;
const bool false = 0;
const bool true = 1;

struct stack {
	int top;
	int size;
	int capacity;
	int *arr;
	void (*push) (stack *, int);
	int (*pop) (stack *);
	int (*peek) (stack *);
	BOOL (*isempty)(stack *);
	void (*delete_stack) (stack *);
};

void push(stack *, int);
int pop(stack *);
int peek(stack *);
void delete_stack(stack *);
stack *make_stack(void);
