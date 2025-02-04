#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>


void insert_node(tree *t, int32_t value)
{
	node *new = build_node(value);

	if(!new)
	{
		puts("Insertion failed.");
		return;
	}
	
#ifdef BT
	node *parent = find_parent(t->binary_tree);
#else
	node *parent = find_parent(t->binary_tree, value);
#endif

	if(!parent)
		t->binary_tree = new;
#ifdef BT
	else if(!(parent->left))
		parent->left = new;
	else
		parent->right = new;
#else
	else if(parent->value < value)
		parent->right = new;
	else
		parent->left = new;
#endif

	puts("Node inserted successfully.");
	return;
}

tree *init_tree(void)
{
	tree *t = (tree *)malloc(sizeof(tree));
	if (t == NULL)
	{
		puts("Unable to created tree. Allocation failed.");
		return NULL;
	}
	t->binary_tree = NULL;
	t->insert_node = &insert_node;
	t->delete_node = &delete_node;
	t->get_height = &get_height;
	t->get_node_level = &get_node_level;
	t->preorder = &preorder;
	t->inorder = &inorder;
	t->postorder = &postorder;
	t->level_order = &level_order;
	t->find_smallest = &find_smallest;
	t->find_largest = &find_largest;
	t->search = &search;
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
        p->next = prev;
        return p;
}

/*int32_t get_height_main(node *root)
{
	if(!root)
		return 0;

	int32_t left = get_height_main(root->left);
	int32_t right = get_height_main(root->right);
	
	return left > right ? left + 1 : right + 1;
}*/

int32_t get_height(tree *t)
{
	return get_height_main(t->binary_tree);
}

int32_t get_node_level(tree *t, int32_t target)
{
	return get_node_level_main(t->binary_tree, target);
}
