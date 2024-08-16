#include <stdio.h>
#include <inttypes.h>
#include <limits.h>
#define MAX 5

void enqueue(int);
int dequeue(void);
uint8_t counter = 0, en = 0, de = 0;
long int queue[MAX];

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
}

void enqueue(int value) {
	if(counter >= MAX) {
		printf("Queue is full.\n");
		return;
	}

	queue[en] = value;
	printf("Added in queue.\n");
	en = (en + 1) % MAX;
	counter++;
}

int dequeue(void) {
	if(counter <= 0) {
		printf("Queue is empty.\n");
		return INT_MAX;
	}

	int return_val = queue[de];
	de = (de + 1) % MAX;
	counter--;

	return return_val;
}
