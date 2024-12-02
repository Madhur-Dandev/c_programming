#include "stack.h"

void push(stack *s, int val) {
	if(s->top >= INT_MAX) {
		printf("Stack overflowed\n");
		return;
	}
	if(s->top >= s->size) {
		// printf("In ");
		int *temp = realloc(
						s->arr,
						sizeof(int) * (s->size * 2)
					);
		if(temp == NULL) {
			printf("Stack overflowed\n");
			return;
		}
		s->arr = temp;
		s->size = s->size * 2;
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

void delete_stack(stack *s) {
	if(s->arr == NULL) {
		return;
	}

	free(s->arr);
	s->top = 0;
	s->size = 0;
	free(s);
	printf("Stack deleted\n");
	return;
}

stack *make_stack() {
	stack *s = malloc(sizeof(stack));
	s->top = 0;
	s->size = 8;
	s->arr = malloc(sizeof(int) * 8);
	s->push = &push;
	s->pop = &pop;
	s->peek = &peek;
	s->delete_stack = &delete_stack;
	return s;
}

