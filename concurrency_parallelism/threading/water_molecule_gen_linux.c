#include "headers.h"
#include <semaphore.h>
#define MOLECULES 100

pthread_barrier_t bar;
pthread_mutex_t mtx;
sem_t sem;
unsigned int water_molecules = 0;

void *oxygen(void *);
void *hydrogen(void *);
void waiting_animation(void);

int main(void) {
	/* total threads will be number of oxygen threads same as
	 * water molecules and hydrogen threads twice as much as
	 * the oxygen thread
	 */
	unsigned int total_thrds = MOLECULES + MOLECULES * 2;
	pthread_t threads[total_thrds];

	/* Initialization starts */
	pthread_mutex_init(&mtx, NULL);
	pthread_barrier_init(&bar, NULL, 3);
	sem_init(&sem, 0, 2);
	/* Initialization ends */
	int res;

	/* Initialize first 50 threads as oxygen threads and rest
	 * as hydrogen threads.
	 */
	for(int i = 0; i < total_thrds; i++) {
		if (i < MOLECULES)
			res = pthread_create(&threads[i], NULL, oxygen, NULL);
		else
			res = pthread_create(&threads[i], NULL, hydrogen, NULL);

		if(res) {
			fprintf(stderr, "Cannot create the thread\n");
			exit(res);
		}
	}

	/* Detaching thread to get them working at brackground */
	for(int i = 0; i < total_thrds; i++) {
		res = pthread_detach(threads[i]);

		if(res)
			fprintf(stderr, "The thread can't be join\n");
	}

	waiting_animation();

	printf("\rMolecules generated: %d\n", water_molecules);

	/* Resource cleanup starts */
	pthread_mutex_destroy(&mtx);
	pthread_barrier_destroy(&bar);
	sem_destroy(&sem);
	/* Resource cleanup ends */

	/* Wait till all background threads complete their execution */
	pthread_exit(NULL);
}

void *oxygen(void *arg) {
	/* Oxygen thread locks the mutex and wait for barrier
	 * to get 3 threads locked in.
	 */
	pthread_mutex_lock(&mtx);

	pthread_barrier_wait(&bar);

	/* After 1 oxygen thread and 2
	 * hydrogen thread acquire
	 * barrier, one water molecule
	 * is made
	 * i.e., water_molecules++;
	 */

	water_molecules++;

	usleep(10000);

	pthread_mutex_unlock(&mtx);

	return NULL;
}

void *hydrogen(void *arg) {
	/* 2 hydrogen thread will allow to enter barrier then
	 * other have to until all barrier gets free.
	 */
	sem_wait(&sem);

	pthread_barrier_wait(&bar);

	sem_post(&sem);

	return NULL;
}

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
