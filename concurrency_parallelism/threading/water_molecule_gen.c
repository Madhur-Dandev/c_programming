#include "headers.h"
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#define MOLECULES 50

#ifdef __APPLE__
/* Apple environment do not support memory barriers.
 * So we have implement the functionality with mutex
 * and condition variables.
 */
pthread_mutex_t barrier_mtx;
pthread_cond_t barrier_cv;
unsigned int current_mem = 0, mem_limit = 3;
#else
pthread_barrier_t bar;
#endif

unsigned int water_molecules = 0;
pthread_mutex_t mtx;
sem_t *sem;

/* Function declaration */
void *oxygen(void *);
void *hydrogen(void *);

#ifdef __APPLE__
void custom_barrier_wait(void);
#endif

void waiting_animation(void);

int main(void) {
	/* total threads will be number of oxygen threads same as
	 * water molecules and hydrogen threads twice as much as
	 * the oxygen thread
	 */
	unsigned int total_thrds = MOLECULES + MOLECULES * 2;
	pthread_t threads[total_thrds];
	int res;

	/* Initialization starts */
	pthread_mutex_init(&mtx, NULL);
#ifdef __APPLE__
	/* Apple environment do not support unnamed semaphore either.
	 * So we have to create named semaphore.
	 */
	pthread_mutex_init(&barrier_mtx, NULL);
	pthread_cond_init(&barrier_cv, NULL);
#else
	pthread_barrier_init(&bar, NULL, 3);
#endif

	sem = sem_open("sem", O_CREAT | O_EXCL, 0644, 2);

	if(sem == SEM_FAILED) {
		char msg[256];

		if(errno == EEXIST)
			strcpy(msg, "Named semaphore alread exist");
		else if(errno == EACCES)
			strcpy(msg, "Unable to access the named semaphore");
		else if(errno == ENOMEM)
			strcpy(msg, "Not enough space to create named semaphore");
		else {}

        perror(msg);
        exit(EXIT_FAILURE);
    }
	/* Initialization ends */

	/* Initialize first 50 threads as oxygen threads and rest
	 * as hydrogen threads.
	 */
	for(int i = 0; i < total_thrds; i++) {
		if(i < MOLECULES)
			res = pthread_create(&threads[i], NULL, oxygen, NULL);
		else
			res = pthread_create(&threads[i], NULL, hydrogen, NULL);

		if(res) {
			fprintf(stderr, "Unable to create enough threads\n");
			exit(res);
		}
	}

	/* Detaching thread to get them working at brackground */
	for(int i = 0; i < total_thrds; i++) {
		res = pthread_detach(threads[i]);

		if(res)
			fprintf(stderr, "Thread cannot undergo detach state.\n");
	}

	waiting_animation();

	printf("\rWater molecules generated: %d\n", water_molecules);

	/* Resource cleanup starts */
	pthread_mutex_destroy(&mtx);

#ifdef __APPLE__
	pthread_mutex_destroy(&barrier_mtx);
	pthread_cond_destroy(&barrier_cv);
#endif

	sem_close(sem);
	sem_unlink("sem");
	/* Resource cleanup ends */

	/* Wait till all background threads complete their execution */
	pthread_exit(NULL);
}

void *oxygen(void *arg) {
	/* Oxygen thread locks the mutex and wait for barrier
	 * to get 3 threads locked in.
	 */
	pthread_mutex_lock(&mtx);

#ifdef __APPLE__
	custom_barrier_wait();
#else
	pthread_barrier_wait(&bar);
#endif

	usleep(50000);

	/* After 1 oxygen thread and 2
	 * hydrogen thread acquire
	 * barrier, one water molecule
	 * is made
	 * i.e., water_molecules++;
	 */
	water_molecules++;

	pthread_mutex_unlock(&mtx);
	return NULL;
}

void *hydrogen(void *arg) {
	/* 2 hydrogen thread will allow to enter barrier then
	 * other have to until all barrier gets free.
	 */
	sem_wait(sem);

#ifdef __APPLE__
	custom_barrier_wait();
#else
	pthread_barrier_wait(&bar);
#endif

	sem_post(sem);
	return NULL;
}

#ifdef __APPLE__
void custom_barrier_wait(void) {
	/* Conterfeit function of actual 'barrier_wait()' function.
	 * Logic:
	 *     If the current member count is less than max size then
	 *     wait for until it get equal.
	 *     When it gen equal of or greater than member limit,
	 *     simply signal every waiting thread on that mutex and
	 *     they will continue execution.
	 */
	pthread_mutex_lock(&barrier_mtx);
	current_mem++;

	if(current_mem >= mem_limit)
		pthread_cond_broadcast(&barrier_cv);
	else
		pthread_cond_wait(&barrier_cv, &barrier_mtx);

	pthread_mutex_unlock(&barrier_mtx);
}
#endif

void waiting_animation(void) {
	/* using carriage return to start from the beginning
	 * of the line and replace the word at the current cursor
	 * position. Thus, creating a loading animation.
	 */
	while(1) {
		printf("\r.");
		fflush(stdout);
		usleep(300000);

		printf("\r..");
		fflush(stdout);
		usleep(300000);

		printf("\r...");
		fflush(stdout);
		usleep(300000);

		printf("\r   ");
		fflush(stdout);
		usleep(300000);
		if(water_molecules == MOLECULES)
			return;
	}
}
