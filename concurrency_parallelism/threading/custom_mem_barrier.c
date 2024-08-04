#include "headers.h"

pthread_mutex_t barrier_mtx;
pthread_cond_t barrier_cv;
unsigned int current_mem = 0, mem_limit = 2;

void custom_barrier_wait(void) {
	pthread_mutex_lock(&barrier_mtx);
	current_mem++;

	if(current_mem >= mem_limit)
		pthread_cond_broadcast(&barrier_cv);
	else
		pthread_cond_wait(&barrier_cv, &barrier_mtx);

	pthread_mutex_unlock(&barrier_mtx);
}

void *first_body(void *arg) {
	printf("A\n");
	custom_barrier_wait();
	return NULL;
}

void *second_body(void *arg) {
	custom_barrier_wait();
	printf("B\n");
	return NULL;
}

void *third_body(void *arg) {
	custom_barrier_wait();
	usleep(1000000);
	printf("C\n");
	return NULL;
}

int main(void) {
	pthread_t t1, t2, t3;

	pthread_mutex_init(&barrier_mtx, NULL);
	pthread_cond_init(&barrier_cv, NULL);


	pthread_create(&t1, NULL, first_body, NULL);
	pthread_create(&t2, NULL, second_body, NULL);
	pthread_create(&t3, NULL, third_body, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	pthread_mutex_destroy(&barrier_mtx);
	pthread_cond_destroy(&barrier_cv);

	return 0;
}
