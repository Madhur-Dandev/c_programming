#include <stdio.h>
#include <stdlib.h>

int min_max(int *, int, short);
double calc_avg(int *, int);
int count(int *, int, int);
int is_sorted(int *, int, short);

int main(void) {
	/*int *arr = (int *) malloc(sizeof(int) * 5);
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 4;
	arr[4] = 5;*/
	
	// int arr[] = {1, 2, 3, 4, 5, 5, 2, 5, 5};
	int arr[] = {1, 2, 3, 4, 5};
	int ret_val = min_max(arr, sizeof(arr)/sizeof(int), 0);
	printf("%d\n", arr[ret_val]);

	printf("%.2f\n", calc_avg(arr, sizeof(arr)/sizeof(int)));

	printf("Count of 5: %d\n", count(arr, sizeof(arr)/sizeof(int), 5));

	printf("Is the array sorted: %s\n", is_sorted(arr, sizeof(arr)/sizeof(int), 1) == 0 ? "False": "True");
	return 0;
}

int min_max(int *arr, int size, short type) {
	// 1 for min, 0 for max
	int val = arr[0]; 
	for(int i = 0; i < size; i++) {
		if(type == 1) {
			if(arr[val] > arr[i])
				val = i;
		} else {
			if(arr[val] < arr[i])
				val = i;
		}
	}

	return val;
}

double calc_avg(int *arr, int size) {
	double avg = 0;
	
	for(int i = 0; i < size; i++) {
		avg += arr[i];
	}

	avg /= size;
	return avg;
}

int count(int *arr, int size, int elem) {
	int count = 0;
	
	for(int i = 0; i < size; i++) {
		if(arr[i] == elem)
			count++;
	}

	return count;
}

int is_sorted(int *arr, int size, short type) {
	// type 0: asc, 1: desc

	for(int i = 1; i < size; i++) {
		if(type == 0) {
			if(arr[i-1] > arr[i])
				return 0;
		}
		else {
			if(arr[i-1] < arr[i])
				return 0;
		}
	}

	return 1;
}
