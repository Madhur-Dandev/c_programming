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
void *waiting_animation(void *);

int main(void) {
	unsigned int total_thrds = MOLECULES + MOLECULES * 2;
	pthread_t threads[total_thrds], anim_thrd;
	pthread_attr_t anim_thrd_attr;
	int res;

	/* initialization starts */
	pthread_mutex_init(&mtx, NULL);
	pthread_mutex_init(&barrier_mtx, NULL);
	pthread_cond_init(&barrier_cv, NULL);
	pthread_attr_init(&anim_thrd_attr);
	pthread_attr_setdetachstate(&anim_thrd_attr, PTHREAD_CREATE_DETACHED);
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

	if(res = pthread_attr_init(&anim_thrd_attr))
		fprintf(stderr, "Cannot initialize thread attribute object\n");
	else {
		res = pthread_attr_setdetachstate(&anim_thrd_attr, PTHREAD_CREATE_DETACHED);

		if(res)
			fprintf(stderr, "Cannot set detach state attribute\n");
		else {
			res = pthread_create(&anim_thrd, &anim_thrd_attr, waiting_animation, NULL);

			if(res)
				fprintf(stderr, "Cannot display waiting animation\n");
		}
	}

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
		res = pthread_join(threads[i], NULL);

		if(res)
			fprintf(stderr, "Thread cannot join\n");
	}

	res = pthread_cancel(anim_thrd);

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
