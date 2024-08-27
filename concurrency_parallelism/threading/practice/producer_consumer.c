#include "../headers.h"
#include <inttypes.h>
#include <limits.h>
#define MAX 5

uint8_t counter = 0, en = 0, de = 0;
long int queue[MAX];
pthread_rwlock_t lock;

void enqueue(int);
int dequeue(void);
void *producer(void *);
void *consumer(void *);

int main(void) {
	pthread_t prod, cons[MAX];
	int res;

	res = pthread_rwlock_init(&lock);
	if(res)
		THROW_ERROR(res, "Unable to create lock\n");"

	res = pthread_create(&prod, NULL, producer, NULL);
	if(res)
		THROW_ERROR(res, "Producer thread cannot be created");

	res = pthread_detach(prod);
	if(res)
		THROW_ERROR(res, "Producer thread unable to detach\n");

	for(int i = 0; i < MAX; i++) {
		res = pthread_create(&cons[i], NULL, consumer, NULL);
		if(res)
			THROW_ERROR(res, "Consumer thread cannot be created");

		res = pthread_detach(cons[i]);
		if(res)
			THROW_ERROR(res, "Consumer thread unable to detach\n");
	}

	pthread_exit(NULL);

	res = pthread_rwlock_destroy(&lock);
	if(res)
		THROW_ERROR(res, "Lock not destroyed\n");
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

void *producer(void *arg) {
	printf("producer\n");
	return NULL;
}


void *consumer(void *arg) {
	printf("consumer\n");
	return NULL;
}
