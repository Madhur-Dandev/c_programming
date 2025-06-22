// graph changed: add an option to specify whether graph is
// directed or undirected.

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
	bool is_directed;
} Graph;

typedef struct stack 
{
	int value;
	struct stack *prev;
	struct stack *next;
} Stack;

Graph *init(bool);
void add_connection(Graph *, int, int, int, bool);
void delete_connection(Graph *, int, int);
void print(Graph *);
Graph *transpose(Graph *);
void push(Stack **, int);
int tell(Stack *);
int pop(Stack **);
void dfs(Graph *, bool *, Stack **);
void freeup(Graph *);

int main(void)
{
	Graph *graph = init(false);
	add_connection(graph, 0, 1, 1, false);
	add_connection(graph, 0, 2, 1, false);
	add_connection(graph, 2, 3, 1, false);
	add_connection(graph, 1, 3, 1, false);
	print(graph);
	Graph *trnps = transpose(graph);
	print(trnps);
	freeup(trnps);
	delete_connection(graph, 1, 2);
	print(graph);
	bool visited[VRTCS];
	memset(visited, 0, sizeof(visited)/sizeof(bool));
	Stack *s = NULL;
	for(int i = 0; i < VRTCS; i++)
	{
		push(&s, i);
		dfs(graph, visited, &s);
	}
	freeup(graph);
	graph = NULL;
	return 0;
}

Graph *init(bool is_directed)
{
	Graph *grph = (Graph *) malloc(sizeof(Graph));
	if(grph == NULL)
	{
		perror("Failed to create the graph");
		return NULL;
	}
	memset(grph, 0, sizeof(Graph));
	grph->is_directed = is_directed;
	return grph;
}

void add_connection(Graph *grph, int vrtx, int lk_vrtx, int wgh, bool trnps)
{
	// vrtx -> vertex
	// lk_vrtx-> link vertex
	// wgh -> weight
	
	if(grph == NULL)
		return;

	CHECK_PARAM(vrtx, lk_vrtx);
	
	Node *new, *new2;
	new = (Node *) malloc(sizeof(Node));
	new2 = NULL;
	if(grph->is_directed == false && trnps == false)
		new2 = (Node *) malloc(sizeof(Node));

	if(new == NULL || (grph->is_directed == false && trnps == false && new2 == NULL))
	{
		perror("Failed to add the edge");
		if(new)
			free(new);
		if(new2)
			free(new2);
		return;
	}

	new->value = lk_vrtx;
	new->weight = wgh;
	new->next = grph->list[vrtx];
	grph->list[vrtx] = new;

	if(new2)
	{
		new2->value = vrtx;
		new2->weight = wgh;
		new2->next = grph->list[lk_vrtx];
		grph->list[lk_vrtx] = new2;
	}
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

	if(grph->is_directed == false)
	{	
		Node *curr2 = grph->list[lk_vrtx];
		Node *prev2 = NULL;
		while(curr2 != NULL)
		{
			if(curr2->value == vrtx)
			{
				if(prev2 == NULL)
					grph->list[lk_vrtx] = curr2->next;
				else
					prev2->next = curr2->next;

				break;
			}
			prev2 = curr2;
			curr2 = curr2->next;
		}
		prev2 = curr2;
		free(curr2);
	}

	puts("Edge deleted");
}

void print(Graph *grph)
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
				if(grph->is_directed)
					printf(" ->");

				printf(" (%d, %d)", curr->value, curr->weight);

				curr = curr->next;
				if(curr != NULL)
					printf(",");
			}
		}
		puts("");
	}
	return;
}

Graph *transpose(Graph *grph)
{
	Graph *trnps = init(grph->is_directed);
	if(trnps == NULL)
		return trnps;

	puts("test starts");
	print(trnps);
	puts("test ends");
	Node *temp, *chd_tmp;
	for(int i = 0; i < VRTCS; i++)
	{
		temp = grph->list[i];
		while(temp)
		{
			/*if(grph->is_directed == false)
			{
				chd_tmp = grph->list[temp->value];
				while(chd_tmp)
				{
					if(
				}
			}*/
			add_connection(trnps, temp->value, i, temp->weight, true);
			temp = temp->next;
		}
	}
	return trnps;
}

void push(Stack **s, int v)
{
	Stack *new = (Stack *) malloc(sizeof(Stack));
	if(new == NULL)
		return;
	
	new->value = v;
	new->prev = new->next = NULL;

	if(*s == NULL)
	{
		*s = new;
	}
	else
	{
		(*s)->next = new;
		new->prev = (*s);
		(*s) = new;
	}
	return;
}

int tell(Stack *s)
{
	return s != NULL ? s->value : -1;
}

int pop(Stack **s)
{
	if(*s == NULL)
		return -1;

	Stack *temp = *s;
	*s = (*s)->prev;
	int v = temp->value;
	free(temp);
	return v;
}

void dfs(Graph *grph, bool *visited, Stack **s)
{
	if(*s ==  NULL)
		return;

	int elem = pop(s);

	if(visited[elem])
		return dfs(grph, visited, s);

	printf("%d\n", elem);
	Node *temp = grph->list[elem];
	while(temp)
	{
		if(!visited[temp->value])
			push(s, temp->value);
		temp = temp->next;
	}
	visited[elem] = true;
	return dfs(grph, visited, s);
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
