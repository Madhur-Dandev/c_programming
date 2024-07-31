#include <stdio.h>

struct swap_bit {
	unsigned int swap : 1;
} swap_bit;

int main(void) {
	int arr[] = {5, 31, 23, 5, 11, 43, 75, 769, 99, 100};

	int pass, i, temp, swapped = 1;
	for(pass = sizeof(arr)/sizeof(arr[0]) - 1; pass >= 0 && swapped; pass--) {
	    swapped = 0;
	    for(i = 0; i <= pass - 1; i++) {
	        if(arr[i] > arr[i+1]) {
	            temp = arr[i];
	            arr[i] = arr[i+1];
	            arr[i+1] = temp;
	            swapped = 1;
	        }
	    }
	}

	for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
		printf("%d ", arr[i]);
	return 0;
}
