#include "binary_tree.h"

node *find_parent(node *root, int32_t value)
{
	if(!root)
		return NULL;

	node *return_value = find_parent(root->value < value ? root->right : root->left,
									value);
	
	return return_value ? return_value : NULL;
}

void insert_node(tree *t, int32_t value)
{
	node *new = build_node(value);

	if(!new)
	{
		puts("Insertion failed.");
		return;
	}
	
	node *parent = find_parent(t->binary_tree, value);
	
	if(!parent)
		parent = new;
	else if(parent->value < value)
		parent->right = new;
	else
		parent->left = new;

	puts("Node inserted successfully.");
	return;
}

void logic(node **root, node **parent, int32_t value)
{
	if((*root)->value == value)
	{
		node *to_free = *root;
		if(parent == NULL)
			*root = (*root)->left;
		else
			(*parent)->left = to_free->left;
		
		node *target = find_parent(to_free->left, to_free->right->value);
		target->right = to_free->right;
		free(to_free);
		return;
	}
	else
	{
		logic((*root)->value < value ? &((*root)->right) : &((*root)->left), &(*root), value);
		return;
	}
}

int32_t delete_node(tree *t, int32_t value)
{
	int32_t return_value;
}
