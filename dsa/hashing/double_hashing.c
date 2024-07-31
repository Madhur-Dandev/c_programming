#include <stdio.h>
#include <inttypes.h>
#include <limits.h>
#define SIZE 10
#define PRIME 7

int arr[SIZE];

int hash_func(int);

int main(void) {
	/*for(int i = 0; i < SIZE; i++) {
		arr[i] = INT_MIN;
	}*/

	printf("%d\n", hash_func(3));
	return 0;
}

int hash_func(int key) {
	uint8_t hash;
	uint8_t multi = 0;

	while(1) {
		hash = ((key % SIZE) + multi * (PRIME - (key % PRIME))) % SIZE;
		printf("%d\n", hash);
		if(arr[hash] == INT_MIN)
			break;
		if(multi >= SIZE)
			return -1;
		multi++;
	}

	return hash;
}
