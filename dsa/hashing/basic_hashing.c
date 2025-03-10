#include <stdio.h>

int fetch_index(int size, int value)
{
	return value % size;
}

void hash_func(int *arr, int size, int value)
{
	int index = fetch_index(size, value);
	arr[index] = value;
}

int main(void)
{
	int num_arr[10];

	hash_func(num_arr, 10, 10);
	
	printf("%d\n", num_arr[fetch_index(10, 10)]);
	
	return 0;
}
