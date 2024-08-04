#include "headers.h"
#include <semaphore.h>
#define MOLECULES 100

pthread_barrier_t bar;
pthread_mutex_t mtx;
sem_t sem;
unsigned int water_molecules = 0;

void *oxygen(void *);
void *hydrogen(void *);
void *waiting_animation(void *);

int main(void) {
	pthread_t threads[MOLECULES + MOLECULES * 2], animation_thread = 0;
	pthread_attr_t anim_thrd_attr;
	/* initialization starts */
	pthread_mutex_init(&mtx, NULL);
	pthread_barrier_init(&bar, NULL, 3);
	sem_init(&sem, 0, 2);
	/* initialization ends */
	int res;

	if(res = pthread_attr_init(&anim_thrd_attr))
		fprintf(stderr, "Cannot initialize thread attribute object\n");
	else {
		res = pthread_attr_setdetachstate(&anim_thrd_attr, PTHREAD_CREATE_DETACHED);

		if(res)
			fprintf(stderr, "Cannot set detach state attribute\n");
		else {
			res = pthread_create(&animation_thread, NULL, waiting_animation, NULL);

			if(res)
				fprintf(stderr, "Cannot display waiting animation\n");
		}
	}
	for(int i = 0; i < MOLECULES + MOLECULES * 2; i++) {
		if (i < MOLECULES)
			res = pthread_create(&threads[i], NULL, oxygen, NULL);
		else
			res = pthread_create(&threads[i], NULL, hydrogen, NULL);

		if(res) {
			fprintf(stderr, "Cannot create the thread\n");
			exit(res);
		}
	}

	for(int i = 0; i < MOLECULES + MOLECULES * 2; i++) {
		res = pthread_join(threads[i], NULL);

		if(res)
			fprintf(stderr, "The thread can't be join\n");
	}

	if(animation_thread)
		pthread_cancel(animation_thread);

	printf("\rMolecules generated: %d\n", water_molecules);

	/* cleanup starts */
	pthread_mutex_destroy(&mtx);
	pthread_barrier_destroy(&bar);
	sem_destroy(&sem);
	/* cleanup ends */
	return 0;
}

void *oxygen(void *arg) {
	pthread_mutex_lock(&mtx);

	pthread_barrier_wait(&bar);

	/* After 1 oxygen thread and 2
	 * hydrogen thread acquire
	 * barrier, one water molecule
	 * is made
	 * i.e., water_molecules++;
	 */

	water_molecules++;

	usleep(100000);

	pthread_mutex_unlock(&mtx);

	return NULL;
}

void *hydrogen(void *arg) {
	sem_wait(&sem);

	pthread_barrier_wait(&bar);

	sem_post(&sem);

	return NULL;
}

void *waiting_animation(void *arg) {
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
	}
}
