#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>

tree *init_tree(void)
{
	tree *t = (tree *)malloc(sizeof(tree));
	if (t == NULL)
	{
		puts("Unable to created tree. Allocation failed.");
		return NULL;
	}
	t->height = 0;
	t->binary_tree = NULL;
	t->insert_node = &insert_node;
	t->delete_node = &delete_node;
	t->find_path = &find_path;
	t->destroy = &destroy;
	return t;
}

node *build_node(int32_t value)
{
	node *new = (node *) malloc(sizeof(node));
	if(new == NULL) {
		puts("Unable to create node");
		return NULL;
	}

	new->value = value;
	new->left = new->right = NULL;
	return new;
}

list *create_list_node(node *n)
{
	if(n == NULL)
		return NULL;
	
	list *new = (list *)malloc(sizeof(list));
	if(new == NULL) {
		return NULL;
	}

	new->n = n;
	new->prev = new->next = NULL;
	return new;
}

void clear_list(list *l) 
{
	if(l == NULL)
		return;
	
	list *temp = l, *to_free;

	while(temp != NULL)
	{
		to_free = temp;
		temp = temp->next;
		free(to_free);
	}

	return;
}

void destroy(tree *t)
{
	if(destroy_tree(t->binary_tree))
	{
		puts("Unable to destroy the tree.");
		return;
	}
	t->binary_tree = NULL;
	free(t);
	puts("Tree Destroyed.");
	return;
}

path_node *make_path_node(path_node *prev, int32_t side)
{
        path_node *p = (path_node *)malloc(sizeof(path_node));
		if(p == NULL)
		{
			return NULL;
		}
		p->position = side;
        /*p->position = side == LEFT
                      ? 'L'
                      : side == RIGHT
                       	? 'R'
                        : '-';*/
        p->next = prev;
        return p;
}
