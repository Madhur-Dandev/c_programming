#include "binary_tree.h"
#include "helper_function.h"

node *find_parent(node *root, int32_t value)
{
	if(!root)
		return NULL;
	
	if(root->value < value)
	{
		if(root->right)
			return find_parent(root->right, value);
		else
			return root;
	}
	else
	{
		if(root->left)
			return find_parent(root->left, value);
		else
			return root;
	}
}

node *handle_deletion(node **root, int32_t value)
{
	if(!(*root))
	{
		return NULL;
	}

	if((*root)->value == value)
	{
		node *to_free = *root;
		if((*root)->left)
		{
			*root = (*root)->left;
			if(to_free->right)
			{
				node *target = find_parent(to_free->left, to_free->right->value);
				if(!target)
					*root = to_free->right;
				else
					target->right = to_free->right;
			}
		}
		else
		{
			*root = (*root)->right;
		}

		return to_free;
	}
	else
	{
		return handle_deletion((*root)->value < value ? &((*root)->right) : &((*root)->left), value);
	}
}

/*void delete_node(tree *t, int32_t value)
{
	node *n = logic(&(t->binary_tree), false, value);
	if(!n)
	{
		puts("No such element.");
	}
	else
	{
		printf("%d\n", n->value);
		free(n);
	}
	return;
}*/

int32_t get_height_main(node *root)
{
	if(!root)
		return 0;

	int32_t left = get_height_main(root->left);
	int32_t right = get_height_main(root->right);

	return (left > right ? left : right) + 1;
}

int32_t get_node_level_main(node *root, int32_t target)
{
	if(!root)
		return -1;
	if(root->value == target)
		return 0;

	int32_t value = get_node_level_main(root->value > target
										? root->left
										: root->right
										, target);
	return value < 0 ? value : value + 1;
}

void preorder_main(node *n)
{
	if (n == NULL)
		return;

	printf("%d\n", n->value);
	preorder_main(n->left);
	preorder_main(n->right);
}

void inorder_main(node *n)
{
	if (n == NULL)
		return;

	inorder_main(n->left);
	printf("%d\n", n->value);
	inorder_main(n->right);
}

void postorder_main(node *n)
{
	if (n == NULL)
		return;

	postorder_main(n->left);
	postorder_main(n->right);
	printf("%d\n", n->value);
}

void level_order_main(node *n, int32_t height)
{
	/* This function is recurse down until the height is 1
	 * when height reaches to 1 then print the value of the
	 * current node.
	 */
	if (n == NULL)
		return;

	if (height == 1)
	{
		printf("%d\n", n->value);
	}
	else
	{
		// as goes down deeper decrement the height by 1 in each step on both the side.
		level_order_main(n->left, height - 1);
		level_order_main(n->right, height - 1);
	}
}

void level_order(tree *t)
{
	// It has to invoke for each level to print.
	int32_t height = t->get_height(t);
	for (int i = 1; i <= height; i++)
	{
		level_order_main(t->binary_tree, i);
	}
	return;
}

int32_t find_smallest_main(node *root)
{
	if(!root)
		return 0;
	
	int32_t value = find_smallest_main(root->left);
	return value == 0 ? root->value : value;
}

int32_t find_largest_main(node *root)
{
	if(!root)
		return 0;
	
	int32_t value = find_largest_main(root->right);
	return value == 0 ? root->value : value;
}

node *search_main(node *root, int32_t target)
{
	if(!root)
		return NULL;
	if(root->value == target)
		return root;

	return search_main(root->value > target ? root->left : root->right, target);
}

path_node *find_path_main(node *root, int32_t target, int32_t side)
{
	if(!root)
		return NULL;
	if(root->value == target)
		return make_path_node(NULL, side);

	path_node *value = find_path_main(root->value > target ? root->left : root->right, target, root->value > target ? LEFT : RIGHT);

	path_node *new = NULL;
	if(value)
		new = make_path_node(value, side);
	return new;
}

path_node *find_path(tree *t, int32_t value)
{
	return find_path_main(t->binary_tree, value, ROOT);
}

int32_t destroy_tree(node *n)
{
	if (n == NULL)
		return 0;

	destroy_tree(n->left);
	destroy_tree(n->right);

	node *to_free;
	if (n->left != NULL)
	{
		to_free = n->left;
		free(to_free);
	}

	if (n->right != NULL)
	{
		to_free = n->right;
		free(to_free);
	}
	return 0;
}
