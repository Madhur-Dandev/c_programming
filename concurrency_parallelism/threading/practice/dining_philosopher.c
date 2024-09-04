#include "../headers.h"
#include <semaphore.h>
#define MAX 5

sem_t forks[MAX], room;

void *body(void *);

int main(void) {
	int res, id[MAX];
	pthread_t philosopher[MAX];

	res = sem_init(&room, 0, 4);
	if(res)
		THROW_ERROR(res, "Cannot create the semaphore for philosopher.")

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

		res = pthread_detach(philosopher[i]);
		if(res)
			THROW_ERROR(res, "Cannot detach the philosopher thread.")
	}


	for(int i = 0; i < MAX; i++) {
		res = sem_destroy(&forks[i]);
		if(res)
			THROW_ERROR(res, "Cannot destroy this fork semaphore.")
	}

	res = sem_destroy(&room);
	if(res)
		THROW_ERROR(res, "Cannot destroy the philosopher semaphore.")

	pthread_exit(NULL);
	return 0;
}

void *body(void *arg) {
	int id, left, right;
	id = left = *((int *) arg);
	right = (left + 1) % MAX;

//	for(int i = 0; i < MAX; i++) {
	while(1) {
		printf("Philosopher %d is thinking\n", id);

		sem_wait(&room);

		sem_wait(&forks[left]);
		sem_wait(&forks[right]);

		printf("Philosopher %d is eating\n", id);

		sem_post(&forks[left]);
		sem_post(&forks[right]);

		sem_post(&room);
	}

//	printf("%d\n", id);
	return NULL;
}
