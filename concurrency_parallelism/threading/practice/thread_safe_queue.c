#include "../headers.h"
#include <inttypes.h>
#include <limits.h>
#define INIT_SIZE 5

pthread_mutex_t mtx;

void enqueue(int);
int dequeue(void);
void *thrd_body(void *);
void *thrd_body2(void *);

uint8_t en, de, change;
int counter = 0;
int *queue;

int main(void) {
	int res, val[5];
	pthread_t thrd[15];

	en = de = change = 0;

	queue = (int *) malloc(sizeof(int) * INIT_SIZE);
	if(queue == NULL)
		THROW_ERROR(errno, "Error in created memory for queue.")

	res = pthread_mutex_init(&mtx, NULL);
	if(res)
		THROW_ERROR(res, "Unable to create the mutex.")

	for(int i = 0; i < 5; i++) {
		val[i] = i + 1;
		res = pthread_create(&thrd[i], NULL, thrd_body, (void *)(val + i));
		if(res)
			THROW_ERROR(res, "Unable to create thread.")
	}

	for(int i = 0; i < 5; i++) {
		res = pthread_join(thrd[i], NULL);
		if(res)
			THROW_ERROR(res, "Unable to join thread.")
	}

	for(int i = 5; i < 15; i++) {
		res = pthread_create(&thrd[i], NULL, thrd_body2, NULL);
		if(res)
			THROW_ERROR(res, "Unable to create thread.")
	}

	for(int i = 5; i < 15; i++) {
		res = pthread_join(thrd[i], NULL);
		if(res)
			THROW_ERROR(res, "Unable to join thread.")
	}

	res = pthread_mutex_destroy(&mtx);
	if(res)
		THROW_ERROR(res, "Unable to destroy the mutex.");

	/*enqueue(1);
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

	enqueue(7);
	enqueue(8);
	enqueue(9);
	enqueue(10);
	enqueue(11);

	printf("%d\n", dequeue());
	printf("%d\n", dequeue());
	printf("%d\n", dequeue());
	printf("%d\n", dequeue());
	printf("%d\n", dequeue());

	enqueue(12);
	enqueue(13);
	enqueue(14);
	enqueue(15);
	enqueue(16);

	printf("%d\n", dequeue());
	printf("%d\n", dequeue());
	printf("%d\n", dequeue());
	printf("%d\n", dequeue());
	printf("%d\n", dequeue()); */

	free(queue);
	return 0;
}

void enqueue(int value) {
	pthread_mutex_lock(&mtx);
	if(counter >= INIT_SIZE && change == 0) {
		printf("Queue is full.\n");
		return;
	}

//	if(counter >= INIT_SIZE && change == 1) {
	if(change == 1) {
		en = (en + 1) % INIT_SIZE;
	}

	queue[en] = value;
	printf("Added in queue.\n");
	counter++;

	if(en + 1 >= INIT_SIZE) {
		change = 0;
		pthread_mutex_unlock(&mtx);
		return;
	}
	else
		en = (en + 1) % INIT_SIZE;

	change = 2;
	pthread_mutex_unlock(&mtx);

	return;
}

int dequeue(void) {
	printf("get\n");
	pthread_mutex_lock(&mtx);
	if(counter <= 0) {
		printf("Queue is empty.\n");
		pthread_mutex_unlock(&mtx);
		return INT_MAX;
	}

	int return_val = queue[de];
	de = (de + 1) % INIT_SIZE;
	if(counter >= INIT_SIZE)
		change = change == 0 ? 1 : 0;
	counter--;

//	printf("val: %d %d\n", change, counter);
	pthread_mutex_unlock(&mtx);

	return return_val;
}

void *thrd_body(void *arg) {
	int val = *((int *) arg);
	enqueue(val);
	pthread_exit(NULL);
}

void *thrd_body2(void *arg) {
	printf("%d\n", dequeue());
	pthread_exit(NULL);
}
