#include "headers.h"
#define THRDS 100

struct arg {
	int data;
	pthread_mutex_t mtx;
};

void *thread_body(void *arg) {
	struct arg *arg_var = (struct arg *) arg;
	pthread_mutex_lock(&(arg_var->mtx));
	arg_var->data++;
	printf("%d\n", arg_var->data);
	pthread_mutex_unlock(&(arg_var->mtx));
	return NULL;
}

int main(void) {
	struct arg arg = {.data = 0};
	pthread_t t[THRDS];
	int res;

	pthread_mutex_init(&(arg.mtx), NULL);

	for(int i = 0; i < THRDS; i++) {
		res = pthread_create(&t[i], NULL, thread_body, (void *)&arg);
		if(res) {
			fprintf(stderr, "Unable to create thread");
			exit(res);
		}
	}

	for(int i = 0; i < THRDS; i++) {
		pthread_join(t[i], NULL);
	}

	pthread_mutex_destroy(&(arg.mtx));

	return 0;
}
