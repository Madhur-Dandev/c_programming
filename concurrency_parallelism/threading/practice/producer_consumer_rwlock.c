#include "../headers.h"
#include <inttypes.h>
#include <limits.h>
#include <semaphore.h>
#define MAX 5

uint8_t counter = 0, en = 0, de = 0, p_done = 0;
int c_read = 0;
long int queue[MAX];
pthread_rwlock_t lock;
sem_t sem;

void enqueue(int);
int dequeue(void);
int top(void);
void *producer(void *);
void *consumer(void *);

int main(void) {
	pthread_t prod, cons[MAX];
	int res;

	res = sem_init(&sem, 0, MAX);
	if(res)
		THROW_ERROR(res, "Unable to create semaphore.");

	res = pthread_rwlock_init(&lock, NULL);
	if(res)
		THROW_ERROR(res, "Unable to create lock.");

	res = pthread_create(&prod, NULL, producer, NULL);
	if(res)
		THROW_ERROR(res, "Producer thread cannot be created.");

	res = pthread_detach(prod);
	if(res)
		THROW_ERROR(res, "Producer thread unable to detach.");

	for(int i = 0; i < MAX; i++) {
		res = pthread_create(&cons[i], NULL, consumer, NULL);
		if(res)
			THROW_ERROR(res, "Consumer thread cannot be created.");

		res = pthread_detach(cons[i]);
		if(res)
			THROW_ERROR(res, "Consumer thread unable to detach.");
	}

	pthread_exit(NULL);

	res = pthread_rwlock_destroy(&lock);
	if(res)
		THROW_ERROR(res, "Lock not destroyed.");

	res = sem_destroy(&sem);
	if(res)
		THROW_ERROR(res, "Unable to destroy semaphore.");

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
		while(p_done == 1) {
			sem_post(&sem);
			sem_wait(&sem);
		}
		pthread_rwlock_wrlock(&lock);

		enqueue(i+1);
		p_done = 1;

		sem_post(&sem);

		pthread_rwlock_unlock(&lock);
	}
//	printf("producer\n");
	return NULL;
}


void *consumer(void *arg) {
	for(int i = 0; i < MAX; i++) {
		while(p_done == 0) {
			sem_post(&sem);
			sem_wait(&sem);
		}

		pthread_rwlock_rdlock(&lock);

		printf("Value read: %d\n", top());
		c_read++;
		if(c_read >= MAX) {
			p_done = 0;
			dequeue();
			sem_post(&sem);
		}
		pthread_rwlock_unlock(&lock);
	}
//	printf("consumer\n");
	return NULL;
}

