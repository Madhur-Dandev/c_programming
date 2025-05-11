#include <stdio.h>
#include <stdlib.h>

#define VRTCS 4
#define CHECK_PARAM(a, b) \
if(a >= VRTCS || b >= VRTCS) \
{ \
	fprintf(stderr, "Invalid vertex\n"); \
	return; \
}

int **init(void);
void add_connection(int **, int, int);
void delete_connection(int **, int, int);
void freeup(int **);

int main(void)
{
	int **mtrx = init();
	add_connection(mtrx, 1, 2);
	delete_connection(mtrx, 1, 2);
	freeup(mtrx);
	return 0;
}

int **init(void)
{
	int **arr = (int **) malloc(sizeof(int *) * VRTCS);
	for(int i = 0; i < VRTCS; i++)
	{
		arr[i] = (int *) malloc(sizeof(int) * VRTCS);
		arr[i][0] = i+1;
	}
	return arr;
}

void add_connection(int **mtrx, int vrtx, int lk_vrtx)
{
	CHECK_PARAM(vrtx, lk_vrtx);
	mtrx[vrtx][lk_vrtx] = 1;
	return;
}

void delete_connection(int **mtrx, int vrtx, int lk_vrtx)
{
	CHECK_PARAM(vrtx, lk_vrtx);
	mtrx[vrtx][lk_vrtx] = 0;
	return;
}

void freeup(int **mtrx)
{
	for(int i = 0; i < VRTCS; i++)
		free(mtrx[i]);
	
	free(mtrx);
}
