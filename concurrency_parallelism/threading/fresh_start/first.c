#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 1000
/*
void *thread_function_1(void *arg)
{
	int thread_no = *((int *) arg);
	printf("Hello from thread %d\n", thread_no);
	return NULL;
} */

struct thread_data
{
	int size;
	int *arr;
};

void *thread_function_2(void *arg)
{
	printf("Here\n");
	int *ptr = (int *) malloc(sizeof(int));
	*ptr = 3;
	return ptr;
}

int main(void)
{
	int arr_size = 30;
	pthread_t thread_id;
	int arr[arr_size];
	for(int i = 0; i < arr_size; i++)
	{
		arr[i] = i+1;
	}

	int no_of_threads = arr_size / 3;
	if(arr_size % 3 != 0)
		no_of_threads++;

	pthread_create(&thread_id, NULL, &thread_function_2, &thread_id);
	int *a; 
	pthread_join(thread_id, (void *) &a);

	printf("%d\n", *a);
	free(a);
	return 0;
}
