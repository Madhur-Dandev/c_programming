#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#define THRD_NUM 5

void *thread_func(void *arg) {
	printf("Hey! Your thread %02d is working...\n", *((int *)(arg)) + 1);
	nanosleep(&(struct timespec){.tv_sec=3, .tv_nsec=500000000}, NULL); /* sleeping thread for 3.5 sec */
	free(arg);
	return NULL;
}

int main(void) {
	pthread_t thread[THRD_NUM];
	int t_create_val, join_val, *pass;

	for(int i = 0; i < THRD_NUM; i++) {
		pass = (int *) malloc(sizeof(int));
		*pass = i;
		t_create_val = pthread_create(&thread[i], 0, thread_func, pass);
		if(t_create_val) {
			fprintf(stderr, "Error creating new thread %d %d\n", t_create_val, EAGAIN);
			return t_create_val;
		}
	}

	for(int i = 0; i < THRD_NUM; i++) {
		/* join_val = pthread_join(thread[i], NULL); /* make thread joined to main thread */
		join_val = pthread_detach(thread[i]); /* make thread detached from main thread */
		if(join_val) {
			fprintf(stderr, "Error joining thread.\n");
			return join_val;
		}
	}

	printf("Main function exits\n");
	pthread_exit(NULL);
	/*return 0;*/
}
