#include "graph.h"

typedef struct graph
{
	int is_weighted;
	int mtrx[VRTCS][VRTCS];
} Graph;

Graph *init(bool);
void add_connection(Graph *, int, int, int);
void delete_connection(Graph *, int, int, int);
void print(Graph *);
Graph *transpose(Graph *);
void freeup(Graph *);

int main(void)
{
	Graph *graph = init(true);
	add_connection(graph, 1, 2, 1);
	add_connection(graph, 1, 0, 1);
	add_connection(graph, 1, 3, 1);
	add_connection(graph, 0, 1, 1);
	add_connection(graph, 0, 2, 99);
	Graph *trnps = transpose(graph);
	print(trnps);
	freeup(trnps);
	//delete_connection(graph, 1, 2, 0);
	print(graph);
	freeup(graph);
	return 0;
}

Graph *init(bool is_weighted)
{
	/*Graph *arr = (Graph *) malloc(sizeof(int *) * VRTCS);
	for(int i = 0; i < VRTCS; i++)
	{
		arr[i] = (int *) malloc(sizeof(int) * VRTCS);
		arr[i][0] = i+1;
	}*/

	Graph *grph = (Graph *) malloc(sizeof(Graph));
	if(grph == NULL)
	{
		perror("Failed to create the graph");
		return NULL;
	}
	memset(grph, 0, sizeof(Graph));
	grph->is_weighted = is_weighted;
	return grph;
}

void add_connection(Graph *grph, int vrtx, int lk_vrtx, int wgh)
{
	// wgh -> weight
	CHECK_PARAM(vrtx, lk_vrtx);
	grph->mtrx[vrtx][lk_vrtx] = grph->is_weighted ? wgh : 1;
	printf("Connection added from %d to %d\n", vrtx, lk_vrtx);
	return;
}

void delete_connection(Graph *grph, int vrtx, int lk_vrtx, int wgh)
{
	// wgh -> weight
	CHECK_PARAM(vrtx, lk_vrtx);
	grph->mtrx[vrtx][lk_vrtx] = 0;
	printf("Connection deleted from %d to %d\n", vrtx, lk_vrtx);
	return;
}

void print(Graph *grph)
{
	printf("  ");
	for(int i = 0; i < VRTCS; i++)
	{
		printf(" %2d", i);
	}
	puts("");

	for(int i = 0; i < VRTCS; i++)
	{
		printf("%2d", i);
		for(int j = 0; j < VRTCS; j++)
		{
			printf(" %2d", grph->mtrx[i][j]);
		}
		puts("");
	}
}

Graph *transpose(Graph *grph)
{
	Graph *trnps = init(grph->is_weighted);
	if(trnps == NULL)
		return trnps;

	for(int i = 0; i < VRTCS; i++)
		for(int j = 0; j < VRTCS; j++)
			if(grph->mtrx[i][j] != 0)
				add_connection(trnps, j, i, grph->mtrx[i][j]);

	return trnps;
}

void freeup(Graph *grph)
{	
	free(grph);
	puts("Graph deleted");
}
