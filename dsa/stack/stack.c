#include "stack.h"

void push(stack *s, int val) {
	/*if(s->top >= INT_MAX) {
		printf("Stack overflowed\n");
		return;
	}*/
	if(s->top >= s->capacity) {
		// printf("In ");
		int *temp = realloc(
						s->arr,
						sizeof(int) * (s->capacity * 2)
					);
		if(temp == NULL) {
			printf("Hey! Stack overflowed\n");
			return;
		}
		s->arr = temp;
		s->capacity *= 2;
	}
	s->arr[s->top++] = val;
	return;
}

int pop(stack *s) {
	if(s->top <= 0) {
		printf("Nothing in stack.\n");
		return 0; // expecting this is no 0 value in stack.
	}

	int val = s->arr[--s->top];
	return val;
}

int peek(stack *s) {
	if(s->top <= 0) {
		printf("Nothing in stack\n");
		return 0; // expecting this is no 0 value in stack.
	}
	return s->arr[s->top];
}

bool isEmpty(stack *s) {
	return s->top <= 0;
}

void deleteStack(stack *s) {
	if(s->arr == NULL) {
		return;
	}

	free(s->arr);
	s->top = 0;
	free(s);
	printf("Stack deleted\n");
	return;
}

stack *makeStack() {
	stack *s = malloc(sizeof(stack));
	s->top = 0;
	s->capacity = 8;
	s->arr = (int *) malloc(sizeof(int) * 8);
	s->push = &push;
	s->pop = &pop;
	s->peek = &peek;
	s->deleteStack = &deleteStack;
	return s;
}
