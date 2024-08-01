#include "headers.h"
#include <inttypes.h>
/*#define THRDS 10*/
/* Define a macro for true and false value */
#define TRUE 1
#define FALSE 0

/*
 * Using uint8_t for storing small value for true and false
 * 8 in uint8_t means 8 bits = 1 byte
 */
typedef uint8_t bool_t;

typedef struct {
	/* using done for ensuring A prints before b */
	bool_t done;
	pthread_mutex_t mtx;
	pthread_cond_t c_var;
}shared_state_t;

void shared_state_init(shared_state_t *);
void shared_state_destroy(shared_state_t *);

void *first_body(void *);
void *second_body(void *);

int main(void) {
	shared_state_t ss;
	pthread_t t1, t2;

	shared_state_init(&ss);
	pthread_create(&t1, NULL, first_body, &ss);
	pthread_create(&t2, NULL, second_body, &ss);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	shared_state_destroy(&ss);
	return 0;
}

void shared_state_init(shared_state_t *ss) {
	/* Initializing all default values */
	ss->done = FALSE;
	pthread_mutex_init(&ss->mtx, NULL);
	pthread_cond_init(&ss->c_var, NULL);
}

void shared_state_destroy(shared_state_t *ss) {
	/* Destroy value after use */
	pthread_mutex_destroy(&ss->mtx);
	pthread_cond_destroy(&ss->c_var);
}

void *first_body(void *arg) {
	shared_state_t *ss = (shared_state_t *) arg;
	pthread_mutex_lock(&ss->mtx);
	printf("A\n");

	ss->done = TRUE;

	/* Signal the waiting thread and release the lock */
	pthread_cond_signal(&ss->c_var);
	pthread_mutex_unlock(&ss->mtx);

	return NULL;
}

void *second_body(void *arg) {
	shared_state_t *ss = (shared_state_t *) arg;
	pthread_mutex_lock(&ss->mtx);

	while(!ss->done) {
		/* wait until a another thread signals */
		pthread_cond_wait(&ss->c_var, &ss->mtx);
	}

	printf("B\n");
	pthread_mutex_unlock(&ss->mtx);

	return NULL;
}

