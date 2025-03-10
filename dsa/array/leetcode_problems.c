#include <stdio.h>
#include <stdlib.h>

int *twoSumSorted(int *, int, int, int *);
int remove_duplicates(int *, int);

int main(void) {
	int returnedSize;
	int arr[] = {11, 2, 0, 4, 5};
	int *returnedArr = twoSumSorted(arr, sizeof(arr)/sizeof(int), 11, &returnedSize);

	if(returnedSize == 2) {
		for(int i = 0; i < returnedSize; i++) {
			printf("%d\n", returnedArr[i]);
		}
	}

	puts("");
	int arr2[] = {0, 0, 0, 1, 2, 2, 3, 3, 3, 3, 4, 4};
	remove_duplicates(arr2, sizeof(arr2)/sizeof(int));
	return 0;
}

int* twoSumSorted(int* nums, int numsSize, int target, int* returnSize) {
    int left = 0, right = numsSize - 1;

    if (!((numsSize >= 2 && numsSize <= 10000) &&
          (target >= -1000000000 && target <= 1000000000))) {
        *returnSize = 0;
        return NULL;
    }
    while (left < right) {
        if ((nums[left] + nums[right]) > target) {
			right--;
        } else if((nums[left] + nums[right]) < target) {
			left++;
		}else {
            *returnSize = 2;
            int* returnArr = malloc(sizeof(int) * 2);
            returnArr[0] = left;
            returnArr[1] = right;
            return returnArr;
        }
    }

    *returnSize = 0;
    return NULL;
}

int remove_duplicates(int *arr, int size)
{
	int last, current;
	last = current = 0;

	for(int i = 1; i < size; i++)
	{
		if(arr[last] != arr[i])
		{
			last++;
			arr[last] = arr[i];
		}
	}

	return last+1;
}
