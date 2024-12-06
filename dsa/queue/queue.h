#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct queue queue;

struct queue {
	int size;
	int capacity;
	int front;
	int end;
	int fixed;
	int *arr;
	void (*enqueue)(queue *, int);
	int (*dequeue)(queue *);
	int (*peek)(queue *);
	bool (*isEmpty)(queue *);
	bool (*isFull)(queue *);
	void (*destroy)(queue *);
};

queue *init_queue(bool);
void enqueue(queue *, int);
int dequeue(queue *);
int peek(queue *);
bool isEmpty(queue *);
bool isFull(queue *);
void destroy(queue *);
