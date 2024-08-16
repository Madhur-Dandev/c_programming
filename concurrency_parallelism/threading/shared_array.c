#include "headers.h"
#include <string.h>
#define CHECK_RESULT(r, msg)\
if(r) {\
	fprintf(stderr, "%s", msg);\
	exit(r);\
}

char *cipher;
unsigned int cipher_len = 52;
pthread_barrier_t bar, bar_populate, bar_print;
int print_done = 0;

void *allocate(void *);
void *populate(void *);
void *print(void *);
void *deallocate(void *);

int main(void) {
	int res;
	char *even = "even";
	char *odd = "odd";

	if( (res = pthread_barrier_init(&bar, NULL, 5)) ||
		(res = pthread_barrier_init(&bar_populate, NULL, 3)) ||
	   	(res = pthread_barrier_init(&bar_print, NULL, 2)) )
  		CHECK_RESULT(res, "Cannot initializa barrier object\n");

	pthread_t t1, t2, t3, t4, t5;

	if( (res = pthread_create(&t1, NULL, allocate, NULL)) ||
		(res = pthread_create(&t2, NULL, populate, (void *) even)) ||
		(res = pthread_create(&t3, NULL, populate, (void *) odd)) ||
		(res = pthread_create(&t4, NULL, print, NULL)) ||
		(res = pthread_create(&t5, NULL, deallocate, NULL)) )
		CHECK_RESULT(res, "Unable to create enough threads for execution\n");

	res = pthread_join(t1, NULL); CHECK_RESULT(res, "Thread cannot join\n");
	res = pthread_join(t2, NULL); CHECK_RESULT(res, "Thread cannot join\n");
	res = pthread_join(t3, NULL); CHECK_RESULT(res, "Thread cannot join\n");
	res = pthread_join(t4, NULL); CHECK_RESULT(res, "Thread cannot join\n");
	res = pthread_join(t5, NULL); CHECK_RESULT(res, "Thread cannot join\n");

	res = pthread_barrier_destroy(&bar); CHECK_RESULT(res, "Common barrier cleanup failed\n");
	res = pthread_barrier_destroy(&bar_populate); CHECK_RESULT(res, "Populate barrier cleanup failed\n");
	res = pthread_barrier_destroy(&bar_print); CHECK_RESULT(res, "Print barrier cleanup failed\n");

	return 0;
}

void *allocate(void *arg) {
	cipher = (char *) malloc(cipher_len * sizeof(char));

	if(!cipher) {
		CHECK_RESULT(ENOMEM, "Insufficient memory\n");
	}

	pthread_barrier_wait(&bar);

	return NULL;
}

void *populate(void *arg) {
	pthread_barrier_wait(&bar);
	char *mode = (char *) arg;

	char c = 'a';
	size_t start_index = 1;
	if (strcmp(mode, "even")) {
		c = 'Z';
		start_index = 0;
	}

	for (size_t i = start_index; i < cipher_len; i += 2) {
		cipher[i] = strcmp(mode, "even") ? c-- : c++;
	}

	cipher[cipher_len] = '\0';

	pthread_barrier_wait(&bar_populate);

	return NULL;
}

void *print(void *arg) {
	pthread_barrier_wait(&bar);
	pthread_barrier_wait(&bar_populate);
	printf("%s\n", cipher);
	pthread_barrier_wait(&bar_print);

	return NULL;
}

void *deallocate(void *arg) {
	pthread_barrier_wait(&bar);
	pthread_barrier_wait(&bar_print);
	free(cipher);

	return NULL;
}
