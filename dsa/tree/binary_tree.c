#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "binary_tree.h"

node *find_parent_main(node *root, int32_t *left_size, int32_t *right_size, int32_t *height, int32_t side)
{
	/* recursive function for finding parent node for child
	 * node insertion. This function find the parent node
	 * level-order wise.
	 */

	if (root == NULL)
	{
		return NULL;
	}

	int32_t left_size_current = 0;	  // variable for getting size of left subtree
	int32_t right_size_current = 0;	  // variable for getting size of right subtree
	int32_t height_current_left = 0;  // variable for getting height of left subtree
	int32_t height_current_right = 0; // variable for getting height of right subtree
	node *to_return = NULL;			  // address to return

	node *left = find_parent_main(root->left, &left_size_current, &left_size_current, &height_current_left, LEFT);	   // recurse down left
	node *right = find_parent_main(root->right, &right_size_current, &right_size_current, &height_current_right, RIGHT); // recurse down right

	if (right == NULL)
	{
		to_return = root;
	}
	else
	{
		/* It will check which side to choose based on:
		 * if the both sides have same weight then choose left,
		 * if left is less than right then left or else right
		 * vice versa of second if.
		 */
		int32_t max_subtree_elem;
		if (height_current_left > height_current_right)
		{
			max_subtree_elem = (int32_t)((pow(2, height_current_left + 1) - 2) / 2);
		}
		else
		{
			max_subtree_elem = (int32_t)((pow(2, height_current_right + 1) - 2) / 2);
		}

		if (left_size_current == right_size_current)
		{
			to_return = left;
		}
		else
		{
			if (left_size_current < max_subtree_elem)
			{
				to_return = left;
			}
			else
			{
				to_return = right;
			}
		}
	}

	// this section is to update the value of parent variable
	// with the current child data.
	(*left_size) += left_size_current;
	(*right_size) += right_size_current;

	// this will ensure than weight is added twice by left
	// and right child
	if (side == LEFT)
		(*left_size) += 1;
	if (side == RIGHT)
		(*right_size) += 1;

	// choose greater then add 1 to original parent height
	(*height) = (height_current_left > height_current_right
					 ? height_current_left
					 : height_current_right) +
				1;

	return to_return;
}

node *find_parent(node *root)
{
	int32_t left_size, right_size, height;
	left_size = right_size = height = 0;
	return find_parent_main(root, &left_size, &right_size, &height, ROOT);
}

node *find_parent_node(node *n, int32_t *left_size, int32_t *right_size, int32_t *height, int32_t side)
{
	/* Same as the function use to find the parent element for
	 * insertion, but uses a differnt formula when choosing
	 * between left or right side.
	 */
	if (n == NULL)
	{
		return NULL;
	}

	int32_t left_size_current = 0;
	int32_t right_size_current = 0;
	int32_t height_current_left = 0;
	int32_t height_current_right = 0;

	node *to_return = NULL;

	node *left = find_parent_node(n->left, &left_size_current, &left_size_current, &height_current_left, LEFT);
	node *right = find_parent_node(n->right, &right_size_current, &right_size_current, &height_current_right, RIGHT);

	if (right == NULL)
	{
		to_return = n;
	}
	else
	{
		/* if both left and right side weight is 1, then
		 * return current element
		 * if both sides are equal bu does not weigh 1, then
		 * return right
		 * if nothing works then use this formula to choose
		 * sides: (2^(max_height+1)/2) - ((2^max_height)/2)
		 * if the right side is less than or equal to the
		 * formula then choose left or else right
		 */
		if (left_size_current == 1 && right_size_current == 1)
		{
			to_return = n;
		}
		else if (left_size_current == right_size_current)
		{
			to_return = right;
		}
		else
		{
			if (((((int32_t)pow(2, height_current_left + 1) / 2) - 1) - (((int32_t)pow(2, height_current_left) / 2))) < right_size_current)
			{
				to_return = right;
			}
			else
			{
				to_return = left;
			}
		}
	}

	(*left_size) += left_size_current;
	(*right_size) += right_size_current;
	if (side == LEFT)
		(*left_size) += 1;
	if (side == RIGHT)
		(*right_size) += 1;
	(*height) = height_current_left + 1;

	return to_return;
}

int32_t delete_node(tree *t)
{
	int32_t return_value = 0;
	int32_t left_size = 0;
	int32_t right_size = 0;
	int32_t height = 0;
	node *target_node = find_parent_node(t->binary_tree, &left_size, &right_size, &height, ROOT);

	if (target_node != NULL)
	{
		node *to_free = NULL;
		if (target_node->right != NULL)
		{
			to_free = target_node->right;
			target_node->right = NULL;
		}
		else if (target_node->left != NULL)
		{
			to_free = target_node->left;
			target_node->left = NULL;
		}
		else
		{
			to_free = target_node;
			t->binary_tree = NULL;
		}

		return_value = to_free->value;

		free(to_free);
	}

	return return_value;
}

int32_t get_height_main(node *root)
{
	return !root ? 0 : get_height_main(root->left) + 1;
}

int32_t get_node_level_main(node *root, int32_t target)
{
	if(!root)
		return -1;
	if(root->value == target)
		return 0;

	int32_t left = get_node_level_main(root->left, target);
	int32_t right = get_node_level_main(root->right, target);

	if(left >= 0)
	{
		return left + 1;
	}
	else if(right >= 0)
	{
		return right + 1;
	}
	else
	{
		return -1;
	}
}

void preorder_main(node *n)
{
	if (n == NULL)
		return;

	printf("%d\n", n->value);
	preorder_main(n->left);
	preorder_main(n->right);
}

/*void preorder(tree *t)
{
	puts("Pre-Order");
	preorder_main(t->binary_tree);
	putc('\n', stdout);
	return;
}*/

void inorder_main(node *n)
{
	if (n == NULL)
		return;

	inorder_main(n->left);
	printf("%d\n", n->value);
	inorder_main(n->right);
}

/*void inorder(tree *t)
{
	puts("In-Order");
	inorder_main(t->binary_tree);
	putc('\n', stdout);
	return;
}*/

void postorder_main(node *n)
{
	if (n == NULL)
		return;

	postorder_main(n->left);
	postorder_main(n->right);
	printf("%d\n", n->value);
}

/*void postorder(tree *t)
{
	puts("Post-Order");
	postorder_main(t->binary_tree);
	putc('\n', stdout);
	return;
}*/

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
	int32_t height = t->get_height(t);
	// It has to invoke for each level to print.
	for (int i = 1; i <= height; i++)
	{
		level_order_main(t->binary_tree, i);
	}
	return;
}

int32_t find_smallest_main(node *n)
{
	if (n == NULL)
		return INT_MAX;

	int32_t left = find_smallest_main(n->left);
	int32_t right = find_smallest_main(n->right);

	return left < right
			   ? (left < n->value
					  ? left
					  : n->value)
			   : (right < n->value
					  ? right
					  : n->value);
}

/*int32_t find_smallest(tree *t)
{
	return find_smallest_main(t->binary_tree);
}*/

int32_t find_largest_main(node *n)
{
	if (n == NULL)
		return INT_MIN;
	
	int32_t left = find_largest_main(n->left);
	int32_t right = find_largest_main(n->right);

	return left > right
			   ? (left > n->value
					  ? left
					  : n->value)
			   : (right > n->value
					  ? right
					  : n->value);
}

/*int32_t find_largest(tree *t)
{
	return find_largest_main(t->binary_tree);
}*/

node *search_main(node *n, int32_t value)
{
	if (n == NULL)
	{
		return NULL;
	}

	if (n->value == value)
	{
		return n;
	}

	node *left = search_main(n->left, value);
	node *right = search_main(n->right, value);

	return left != NULL ? left : right;
}

/*node *search(tree *t, int32_t value)
{
	return search_main(t->binary_tree, value);
}*/

path_node *find_path_main(node *n, int32_t value, int32_t side)
{
	/* If will recurse down in tree and check for the node
	 * if found it traverse back while making a singly
	 * linked-list of path direction from the root node.
	 * The past direction will tell: '-': root, 'L': left,
	 * 'R': right
	 * These function is baised toward left. Means if both
	 * sides have the same element then the algoithm will
	 * only choose left.
	 */
	if (n == NULL)
		return NULL;

	if (n->value == value)
	{
		return make_path_node(NULL, side);
	}

	path_node *left = find_path_main(n->left, value, LEFT);
	path_node *right = find_path_main(n->right, value, RIGHT);

	if (left != NULL)
	{
		if (right != NULL)
		{
			// free up the memory of right list
			path_node *temp = right, *to_free;
			while (temp != NULL)
			{
				to_free = temp;
				temp = temp->next;
				free(to_free);
			}
		}
		return make_path_node(left, side);
	}
	else if (left == NULL && right != NULL)
	{
		return make_path_node(right, side);
	}
	else
	{
		return NULL;
	}
}

path_node *find_path(tree *t, int32_t value)
{
	// This algorithm is baised towards left side search
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
