#include <stdio.h>
#include <limits.h>
#include <inttypes.h>
#include <math.h>
#define SIZE 10

int arr[SIZE];

int hash_func(int);
int main(void) {
	for(int i = 0; i < SIZE; i++) {
		arr[i] = INT_MIN;
	}
	printf("%d\n", hash_func(2));
	return 0;
}

int hash_func(int key) {
	uint8_t hash = 0;
	uint8_t val = 0;
	while(1) {
		hash = (key + (int) pow(val, 2)) % SIZE;
		if(arr[hash] == INT_MIN)
			break;
		if(val == 10)
			return -1;
		val++;
	}

	return hash;
}
