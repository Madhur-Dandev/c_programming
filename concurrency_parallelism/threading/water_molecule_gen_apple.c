/* For logic and implementation view
 * water_molecule_gen_linux.c or
 * water_molecule_gen.c
 */

#include "headers.h"
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#define MOLECULES 50

pthread_mutex_t barrier_mtx, mtx;
pthread_cond_t barrier_cv;
sem_t *named_sem;
unsigned int current_mem = 0, mem_limit = 3, water_molecules = 0;

void *oxygen(void *);
void *hydrogen(void *);
void custom_barrier_wait(void);
void waiting_animation(void);

int main(void) {
	unsigned int total_thrds = MOLECULES + MOLECULES * 2;
	pthread_t threads[total_thrds];
	int res;

	/* initialization starts */
	pthread_mutex_init(&mtx, NULL);
	pthread_mutex_init(&barrier_mtx, NULL);
	pthread_cond_init(&barrier_cv, NULL);
	named_sem = sem_open("sem", O_CREAT | O_EXCL, 0644, 2);
	if (named_sem == SEM_FAILED) {
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
	/* initialization ends */

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

	for(int i = 0; i < total_thrds; i++) {
		res = pthread_detach(threads[i]);

		if(res)
			fprintf(stderr, "Thread cannot get into detached state.\n");
	}

	waiting_animation();

	printf("\rWater molecules generated: %d\n", water_molecules);

	/* initialization starts */
	pthread_mutex_destroy(&mtx);
	pthread_mutex_destroy(&barrier_mtx);
	pthread_cond_destroy(&barrier_cv);
	sem_close(named_sem);
	sem_unlink("sem");
	/* initialization ends */

	pthread_exit(NULL);
}

void *oxygen(void *arg) {
	pthread_mutex_lock(&mtx);

	custom_barrier_wait();

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
	sem_wait(named_sem);

	custom_barrier_wait();

	sem_post(named_sem);
	return NULL;
}

void custom_barrier_wait(void) {
	pthread_mutex_lock(&barrier_mtx);
	current_mem++;

	if(current_mem >= mem_limit)
		pthread_cond_broadcast(&barrier_cv);
	else
		pthread_cond_wait(&barrier_cv, &barrier_mtx);

	pthread_mutex_unlock(&barrier_mtx);
}

void waiting_animation(void) {
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
