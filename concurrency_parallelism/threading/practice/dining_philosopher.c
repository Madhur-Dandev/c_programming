#include "../headers.h"
#include <semaphore.h>
#define MAX 5

sem_t forks[MAX], room;
pthread_mutex_t mtx;
pthread_barrier_t bar;

void *body(void *);

int main(void) {
	int res, id[MAX];
	pthread_t philosopher[MAX];

	res = pthread_mutex_init(&mtx, NULL);
	if(res)
		THROW_ERROR(res, "Cannot create the mutex.")

	res = pthread_barrier_init(&bar, NULL, MAX);
	if(res)
		THROW_ERROR(res, "Cannot create the memory barrier.")

	for(int i = 0; i < MAX; i++) {
		res = sem_init(&forks[i], 0, 1);
		if(res)
			THROW_ERROR(res, "Cannot create the semaphore for fork.")
	}

	for(int i = 0; i < MAX; i++) {
		id[i] = i + 1;
		res = pthread_create(&philosopher[i], NULL, body, (void *) &id[i]);
		if(res)
			THROW_ERROR(res, "Cannot create philosopher thread.")
	}

	for(int i = 0; i < MAX; i++) {
		res = pthread_join(philosopher[i], NULL);
		if(res)
			THROW_ERROR(res, "Cannot join the philosopher thread.")
	}


	for(int i = 0; i < MAX; i++) {
		res = sem_destroy(&forks[i]);
		if(res)
			THROW_ERROR(res, "Cannot destroy this fork semaphore.")
	}

	res = pthread_barrier_destroy(&bar);
	if(res)
		THROW_ERROR(res, "Cannot destroy the memory barrier.")

	pthread_exit(NULL);
	return 0;
}

void *body(void *arg) {
	int id, left, right;
	id  = *((int *) arg);
	left = id - 1;
	right = (id) % MAX;

//	for(int i = 0; i < MAX; i++) {
	while(1) {
		printf("Philosopher %d is thinking\n", id);
		pthread_barrier_wait(&bar);
		pthread_mutex_lock(&mtx);

		sem_wait(&forks[left]);
		sem_wait(&forks[right]);

		printf("Philosopher %d is eating\n", id);

		sem_post(&forks[left]);
		sem_post(&forks[right]);

		pthread_mutex_unlock(&mtx);
		pthread_barrier_wait(&bar);
	}

	return NULL;
}
