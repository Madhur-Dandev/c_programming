#include "queue.h"
#include <limits.h>

queue *init_queue(bool);
void enqueue(queue *, int);
int dequeue(queue *);
int peek(queue *);
bool isEmpty(queue *);
bool isFull(queue *);
void destroy(queue *);

/*uint8_t counter = 0, en = 0, de = 0;
long int queue[MAX];*/

/*
int main(void) {
	enqueue(1);
	enqueue(2);
	enqueue(3);
	enqueue(4);
	enqueue(5);

	enqueue(6);

	printf("%d\n", dequeue());
	printf("%d\n", dequeue());
	printf("%d\n", dequeue());
	printf("%d\n", dequeue());
	printf("%d\n", dequeue());
	printf("%d\n", dequeue());

	return 0;
}*/

queue *init_queue(bool fixed) {
	queue *q = (queue *) malloc(sizeof(queue));
	q->size = 0;
	q->capacity = 8;
	q->front = 0;
	q->end = 0;
	q->fixed = fixed;
	q->arr = (int *) malloc(sizeof(int) * 8);
	q->enqueue = &enqueue;
	q->dequeue = &dequeue;
	q->peek = &peek;
	q->isEmpty = &isEmpty;
	q->isFull = &isFull;
	q->destroy = &destroy;

	return q;
}

void enqueue(queue *q, int value) {
	if(q->size >= q->capacity) {
		if(q->fixed) {
			printf("Queue is full.\n");
			return;
		}
		
		int *temp = (int *) realloc(q->arr, sizeof(int) * q->capacity * 2);
		if(temp == NULL) {
			printf("Unable to expand the queue size\n");
			return;
		}

		q->arr = temp;
		q->capacity *= 2;
	}

	q->arr[q->end] = value;
	printf("Added in queue.\n");
	q->end = (q->end + 1) % q->capacity;
	q->size++;
	return;
}

int dequeue(queue *q) {
	if(q->size <= 0) {
		printf("Queue is empty.\n");
		return INT_MAX;
	}

	int ret_val = q->arr[q->front];
	q->front = (q->front + 1) % q->capacity;
	q->size--;

	return ret_val;
}

int peek(queue *q) {
	if(q->size <= 0) {
		printf("Queue empty\n");
		return INT_MAX;
	}
	int ret_val = q->arr[q->front];
	return ret_val;
}

bool isEmpty(queue *q) {
	return q->size <= 0;
}

bool isFull(queue *q) {
	return q->size >= q->capacity;
}

void destroy(queue *q) {
	free(q->arr);
	q->size = 0;
	q->capacity = 0;
	q->front = 0;
	q->end = 0;
	q->fixed = 0;
	q->arr = NULL;
	q->enqueue = NULL;
	q->dequeue = NULL;
	q->peek = NULL;
	q->isEmpty = NULL;
	q->isFull = NULL;
	q->destroy = NULL;
	return;
}
