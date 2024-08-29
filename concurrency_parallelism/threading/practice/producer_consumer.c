#include "../headers.h"
#include <inttypes.h>
#include <limits.h>
#define MAX 5

uint8_t counter = 0, en = 0, de = 0, p_done = 0;
int c_read = 0;
long int queue[MAX];
pthread_rwlock_t lock;
pthread_mutex_t mtx;
pthread_cond_t cnd;

void enqueue(int);
int dequeue(void);
int top(void);
void *producer(void *);
void *consumer(void *);

int main(void) {
	pthread_t prod, cons[MAX];
	int res;

	res = pthread_rwlock_init(&lock, NULL);
	if(res)
		THROW_ERROR(res, "Unable to create lock\n");

	res = pthread_mutex_init(&mtx, NULL);
	if(res)
		THROW_ERROR(res, "Unable to create mutex\n");

	res = pthread_cond_init(&cnd, NULL);
	if(res)
		THROW_ERROR(res, "Unable to create condition variable.\n");

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
		THROW_ERROR(res, "Lock not destroyed.\n");

	res = pthread_mutex_destroy(&mtx);
	if(res)
		THROW_ERROR(res, "Mutex not destroyed.\n");

	res = pthread_cond_destroy(&cnd);
	if(res)
		THROW_ERROR(res, "Condition variable not destroyed.\n");
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

int top(void) {
	return queue[de];
}

void *producer(void *arg) {
	for(int i = 0; i < MAX; i++) {
//		pthread_rwlock_wrlock(&lock);
		pthread_mutex_lock(&mtx);
		while(p_done == 1)
			pthread_cond_wait(&cnd, &mtx);

		enqueue(i+1);
		p_done = 1;

		pthread_cond_broadcast(&cnd);
		pthread_mutex_unlock(&mtx);
//		pthread_rwlock_unlock(&lock);
	}
//	printf("producer\n");
	return NULL;
}


void *consumer(void *arg) {
//	pthread_rwlock_rdlock(&lock);
	for(int i = 0; i < MAX; i++) {
		pthread_mutex_lock(&mtx);
		while(p_done == 0)
			pthread_cond_wait(&cnd, &mtx);

		printf("Value read: %d\n", top());
		c_read++;
		if(c_read >= MAX) {
			c_read = p_done = 0;
			dequeue();
			pthread_cond_broadcast(&cnd);
		}
		pthread_mutex_unlock(&mtx);
	}
//	printf("consumer\n");
	return NULL;
}
