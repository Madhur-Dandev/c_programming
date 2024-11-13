#include <stdio.h>
#include <stdlib.h>

int *merge(int *, int *, int, int);
void rearrange(int *, int);
void rearrange_preserve(int *, int);

int main(void) {
	/*int arr1[] = {1, 2, 3, 4, 5};
	int arr2[] = {6, 7, 8, 9, 10};

	int *merged_arr = merge(arr1, arr2, sizeof(arr1)/sizeof(int), sizeof(arr2)/sizeof(int));

	for(int i = 0; i < sizeof(arr1)/sizeof(int) + sizeof(arr2)/sizeof(int); i++)
		printf("%d\n", merged_arr[i]);

	free(merged_arr);*/

	int arr[] = {1, 0, 2, -1, -2, -3, 3, -1};
	rearrange(arr, sizeof(arr)/sizeof(int));
	for(int i = 0; i < sizeof(arr)/sizeof(int); i++) {
		printf("%d\n", arr[i]);
	}
	return 0;
}

int *merge(int *arr1, int *arr2, int size1, int size2) {
	int *merged_arr = (int *) malloc(sizeof(int) * (size1 + size2));
	
	if(merged_arr == NULL)
		return NULL;

	for(int i = 0; i < size1 + size2; i++) {
		if(i < size1)
			merged_arr[i] = arr1[i];
		else
			merged_arr[i] = arr2[i-size1];
	}

	return merged_arr;
}

void rearrange(int *arr, int size) {
	int begin = 0, end = size-1;
	int location = -1;
	while(begin <= end) {
		if(arr[begin] < 0)
			begin++;
		else if(arr[end] > 0)
			end--;
		else {
			arr[begin] += arr[end];
			arr[end] = arr[begin] - arr[end];
			arr[begin] -= arr[end];
			begin++;
			end--;
		}
	}
}

void rearrange_preserve(int *arr, int size) {
	// preserving the order of positive elements
	int needle = 0;
	for(int i = 0; i < size; i++) {
		if(arr[i] < 0)
			continue;

		for(int j = i+1; j < size; j++) {
			if(arr[j] < 0) {
				int current_elem = arr[j];
				for(int k = j; k > i; k--) {
					arr[k] += arr[k-1];
					arr[k-1] = arr[k] - arr[k-1];
					arr[k] -= arr[k-1];
				}
				break;
			}
		}
	}

	
	return;
}

