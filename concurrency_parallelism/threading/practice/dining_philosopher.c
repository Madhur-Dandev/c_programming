#include "../headers.h"
#include <semaphore.h>
#define NUM 5

int forks = NUM;
sem_t sem;
void *body(void *);

int main(void) {
	pthread_t philosopher[NUM];
	int res;

	res = sem_init(&sem, 0, 2);
	if(res)
		THROW_ERROR(res, "Cannot create thread for new philosopher.");

	for(int i = 0; i < NUM; i++) {
		int val = i+1;
		res = pthread_create(&philosopher[i], NULL, body, (void *)&val);
		if(res)
			THROW_ERROR(res, "Cannot create thread for new philosopher.");

		res = pthread_detach(philosopher[i]);
		if(res)
			THROW_ERROR(res, "Cannot create thread for new philosopher.");
	}


	/*for(int i = 0; i < NUM; i++) {
		res = pthread_join(philosopher[i], NULL);
		if(res)
			THROW_ERROR(res, "Cannot create thread for new philosopher.");
	}*/

	res = sem_destroy(&sem);
	if(res)
		THROW_ERROR(res, "Cannot create thread for new philosopher.")

	pthread_exit(NULL);

	return 0;
}

void *body(void *arg) {
	int id = *((int *) arg);

	printf("%d", id);

	while(1) {
		while(forks < 2)
			sem_wait(&sem);

		forks -= 2;

		printf("Philosopher %d eat thier chunk\n", id);
		forks += 2;
		usleep(100000);
		sem_post(&sem);
	}
	return NULL;
}
