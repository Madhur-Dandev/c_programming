#include <stdio.h>
#include <float.h>
#include <stdlib.h>

/*struct stack {
	void push(int *, int, int);
};*/

float sum(float, float);
float sub(float, float);
float mul(float, float);
float frc(float, float);
int *gen_arr_int(int);
typedef float (*function_ptr)(float, float); // new type for function pointer

void sort_arr(int *, int, int (*)(int, int));
int ascend(int, int);
int descend(int, int);

struct 
int main(void) {
	// 1. Declare a function pointer
	float (*func_ptr1)(float, float) = &sub;
	int *(*func_ptr2)(int) = &gen_arr_int;
	// done


	// 2. Declare an array of function pointer
	/* Method 1:
	 * by making an type of function pointer,
	 * this method supports implicit sizing.
	 */
	function_ptr ptrs1[] = {&sum, &sub, &mul, &frc};

	/* Method 2:
	 * by directly specifying type of function
	 * for the function pointer. But it do not
	 * suppots implicit sizing.
	 */
	float (*ptrs2[4])(float, float) = {&sum, &sub, &mul, &frc};
	// done


	// 3. Implement a callback function
	int arr[] = {11, 0, 5, 2, 6};
	sort_arr(arr, 5, &ascend);
	for(int i = 0; i < 5; i++)
		printf("%d\n", arr[i]);
	puts("");
	// done

	return 0;
}

float sum(float a, float b) {
	if (a > 0 && b > FLT_MAX - a) {
        // Overflow detected
        return FLT_MAX;
    }
	if (a < 0 && b < FLT_MIN - a) {
        // Underflow detected
        return FLT_MIN;
    }

    return a + b;
}

float sub(float a, float b) {
	if(a > 0 && (FLT_MAX + a) < b) {
		printf("Overflow detected\n");
		return FLT_MIN;
	}
	if(a < 0 && (FLT_MAX + a) < b) {
		printf("Overflow detected\n");
		return FLT_MIN;
	}
	printf("here\n");
	return a - b;
}

float mul(float a, float b) {
	if(b > 0 && FLT_MAX / a < b) {
		printf("Overflow detected\n");
		return FLT_MIN;
	}
	if(b < 0 && FLT_MAX / a > b) {
		printf("Overflow detected\n");
		return FLT_MIN;
	}
	return a * b;
}

float frc(float a, float b) {
	return a / b;
}

int *gen_arr_int(int size) {
	int *arr = malloc(sizeof(int) * size);
	printf("Array allocated\n");
	return arr;	
}

void sort_ascending(int *arr, int size) {
	return;
}

void sort_arr(int *arr, int size, int (*callback)(int, int)) {
	for(int i = 0; i < size; i++) {
		for(int j = i+1; j < size; j++) {
			if(callback(arr[i], arr[j]) > 0) {
				arr[i] += arr[j];
				arr[j] = arr[i] - arr[j];
				arr[i] -= arr[j];
			}
		}
	}
}

int ascend(int a, int b) {
	return a - b;
}

int descend(int a, int b) {
	return b - a;
}
