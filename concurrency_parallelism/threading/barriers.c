#include "headers.h"

typedef struct {
	int data;
	short done;
	pthread_barrier_t bar;
	pthread_mutex_t mtx;
	pthread_cond_t cv;
} arg_var;

void *inc_one(void *);
void *inc_two(void *);
void *dec_three(void *);

int main(void) {
	pthread_t t1, t2, t3;
	arg_var arg = {.data = 0, .done = 0};
	pthread_barrier_init(&arg.bar, NULL, 3);
	pthread_mutex_init(&arg.mtx, NULL);
	pthread_cond_init(&arg.cv, NULL);

	if(pthread_create(&t1, NULL, inc_one, (void *) &arg) || pthread_create(&t2, NULL, inc_two, (void *) &arg) || pthread_create(&t3, NULL, dec_three, (void *) &arg)) {
		fprintf(stderr, "Unable to create thread\n");
		exit(errno);
	}

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	pthread_barrier_destroy(&arg.bar);
	pthread_mutex_destroy(&arg.mtx);
	pthread_cond_destroy(&arg.cv);
	return 0;
}

void *inc_one(void *arg) {
	arg_var *a = (arg_var *) arg;
	a->data++;
	printf("%d\n", a->data);

	pthread_barrier_wait(&a->bar);
	return NULL;
}

void *inc_two(void *arg) {
	arg_var *a = (arg_var *) arg;
	pthread_barrier_wait(&a->bar);
	pthread_mutex_lock(&a->mtx);

	a->data += 2;
	printf("%d\n", a->data);
	a->done = 1;

	pthread_cond_signal(&a->cv);
	pthread_mutex_unlock(&a->mtx);
	return NULL;
}

void *dec_three(void *arg) {
	arg_var *a = (arg_var *) arg;
	pthread_barrier_wait(&a->bar);
	pthread_mutex_lock(&a->mtx);

	while(!a->done)
		pthread_cond_wait(&a->cv, &a->mtx);

	a->data -= 3;
	printf("%d\n", a->data);

	pthread_mutex_unlock(&a->mtx);
	return NULL;
}
