#include "headers.h"
#define THRD_NUM 10

void *thread_logic(void *);

int main(void) {
	pthread_t threads[THRD_NUM];
	pthread_attr_t thread_attrs[THRD_NUM];
	int create, attr_init;

	for(int i = 0; i < THRD_NUM; i++) {
		if(attr_init = pthread_attr_init(thread_attrs + i)) {
			fprintf(stderr, "Unable to allocate space for thread attribute\n");
			exit(attr_init);
		}

		pthread_attr_setdetachstate(thread_attrs + i, PTHREAD_CREATE_DETACHED);

		if(create = pthread_create(threads + i, thread_attrs + i, thread_logic, NULL)) {
			fprintf(stderr, "Unable to spawn a new thread\n");
			exit(create);
		}
	}

	printf("Waiting for detached threads to finish...\n");
	pthread_exit(NULL);
}

void *thread_logic(void *arg) {
	sleep(1);
	printf("Thread executed\n");
	return NULL;
}
