#include "graph.h"

typedef struct adj_list_node
{
	int value;
	int weight;
	struct adj_list_node *next;
} Node;

typedef struct graph_detail
{
	Node *list[VRTCS];
	bool is_weighted;
} Graph;

Graph *init(bool);
void add_connection(Graph *, int, int, int);
void delete_connection(Graph *, int, int);
void print(Graph *, bool);
void freeup(Graph *);

int main(void)
{
	Graph *graph = init(true);
	add_connection(graph, 1, 2, 1);
	add_connection(graph, 2, 1, 1);
	add_connection(graph, 1, 0, 1);
	add_connection(graph, 0, 1, 1);
	print(graph, true);
	delete_connection(graph, 1, 2);
	print(graph, false);
	freeup(graph);
	graph = NULL;
	return 0;
}

Graph *init(bool is_weighted)
{
	Graph *grph = (Graph *) malloc(sizeof(Graph));
	if(grph == NULL)
	{
		perror("Failed to add the edge");
		return NULL;
	}
	memset(grph, 0, sizeof(Graph));
	grph->is_weighted = is_weighted;
	return grph;
}

void add_connection(Graph *grph, int vrtx, int lk_vrtx, int wgh)
{
	// vrtx -> vertex
	// lk_vrtx-> link vertex
	// wgh -> weight
	
	if(grph == NULL)
		return;

	CHECK_PARAM(vrtx, lk_vrtx);
	
	Node *new = (Node *) malloc(sizeof(Node));
	if(new == NULL)
	{
		perror("Failed to add the edge");
		return;
	}
	new->value = lk_vrtx;
	new->weight = grph->is_weighted ? wgh : 1;
	new->next = grph->list[vrtx];
	grph->list[vrtx] = new;
	printf("Edge added from %d from %d\n", vrtx, lk_vrtx);
	return;
}

void delete_connection(Graph *grph, int vrtx, int lk_vrtx)
{
	// vrtx -> vertex
	// lk_vrtx-> link vertex

	if(grph == NULL)
		return;
	
	CHECK_PARAM(vrtx, lk_vrtx);

	if(grph->list[vrtx] == NULL)
	{
		fprintf(stderr, "Error: No edge from %d to %d\n", vrtx, lk_vrtx);
	}

	Node *curr = grph->list[vrtx];
	Node *prev = NULL;
	while(curr != NULL)
	{
		if(curr->value == lk_vrtx)
		{
			if(prev == NULL)
				grph->list[vrtx] = curr->next;
			else
				prev->next = curr->next;

			break;
		}
		prev = curr;
		curr = curr->next;
	}
	prev = curr;
	free(curr);

	puts("Edge deleted");
}

void print(Graph *grph, bool directed)
{
	for(int i = 0; i < VRTCS; i++)
	{
		printf("Vertex %d:", i);
		if(grph->list[i] == NULL)
			printf(" No edge");
		else
		{
			Node *curr = grph->list[i];
			while(curr != NULL)
			{
				if(directed)
					printf(" ->");

				if(grph->is_weighted)
					printf(" (%d, %d)", curr->value, curr->weight);
				else
					printf(" %d", curr->value);

				curr = curr->next;
				if(curr != NULL)
					printf(",");
			}
		}
		puts("");
	}
	return;
}

void freeup(Graph *grph)
{
	Node *curr, *to_free;

	for(int i = 0; i < VRTCS; i++)
	{
		if(grph->list[i] != NULL)
		{
			curr = grph->list[i];
			while(curr != NULL)
			{
				to_free = curr;
				curr = curr->next;
				free(to_free);
			}
		}
	}

	free(grph);
	puts("Graph deleted");
	return;
}
