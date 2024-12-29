#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "binary_tree.h"

// struct tracker tracker;

tree *init_tree(void) {
	tree *t = (tree *) malloc(sizeof(tree));
	if(t == NULL) {
		puts("Unable to created tree. Allocation failed.");
		return NULL;
	}
	t->binary_tree = NULL;
	t->insert_node = &insert_node;
	t->delete_node = &delete_node;
	t->destroy = &destroy;
	return t;
}

node *find_node(node *n, int32_t *left_size, int32_t *right_size, int32_t *level, int32_t side) {
	if(n == NULL) {
		return NULL;
	}

	int32_t left_size_current = 0;
	int32_t right_size_current = 0;
	int32_t level_current_left = 0;
	int32_t level_current_right = 0;
	node *to_return = NULL;

	node *left = find_node(n->left, &left_size_current, &left_size_current, &level_current_left, LEFT);
	node *right = find_node(n->right, &right_size_current, &right_size_current, &level_current_right, RIGHT);
	
	if(right == NULL) {
		to_return = n;
	}
	else {
		int32_t max_subtree_elem;
		if(level_current_left > level_current_right) {
			max_subtree_elem = (int32_t) ((pow(2, level_current_left + 1) - 2) / 2);
		}
		else {
			max_subtree_elem = (int32_t) ((pow(2, level_current_right + 1) - 2) / 2);
		}

		if(left_size_current == right_size_current) {	
			to_return = left;
		}
		else {
			if(left_size_current < max_subtree_elem) {
				to_return = left;
			}
			else {
				to_return = right;
			}
		}
	}

	(*left_size) += left_size_current;
	(*right_size) += right_size_current;
	if(side == LEFT)
		(*left_size) += 1;
	if(side == RIGHT)
		(*right_size) += 1;
	(*level) = (level_current_left > level_current_right
			   ? level_current_left
			   : level_current_right) + 1;

	return to_return;
}

void insert_node(tree *t, int32_t value) {
	node *n = (node *) malloc(sizeof(node));
	if(n == NULL) {
		puts("Cannot insert node");
		return;
	}
	
	n->val = value;
	n->left = n->right = NULL;
	
	if(t->binary_tree == NULL) {
		t->binary_tree = n;
		return;
	}
	
	int32_t left_size, right_size, level;
	left_size = right_size = level = 0;
	node *target_node = find_node(t->binary_tree, &left_size, &right_size, &level, ROOT);
	//printf("%d %d %d\n", target_node->val, left_size, right_size);
	if(target_node->left == NULL)
		target_node->left = n;
	else
		target_node->right = n;

	return;
}

node *find_parent_node(node *n, int32_t *left_size, int32_t *right_size, int32_t *level, int32_t side) {
	if(n == NULL) {
		return NULL;
	}

	int32_t left_size_current = 0;
	int32_t right_size_current = 0;
	int32_t level_current_left = 0;
	int32_t level_current_right = 0;

	node *to_return = NULL;

	node *left = find_parent_node(n->left, &left_size_current, &left_size_current, &level_current_left, LEFT);
	node *right = find_parent_node(n->right, &right_size_current, &right_size_current, &level_current_right, RIGHT);
	
	if(right == NULL) {
		to_return = n;
	}
	else {
		if(left_size_current == 1 && right_size_current == 1) {
			to_return = n;
		}
		else if(left_size_current == right_size_current) {	
			to_return = right;
		}
		else {
			if(((((int32_t)pow(2, level_current_left + 1) / 2) - 1) - (((int32_t) pow(2, level_current_left)/2))) < right_size_current) {
				to_return = right;
			}
			else {
				to_return = left;
			}
		}
	}

	(*left_size) += left_size_current;
	(*right_size) += right_size_current;
	if(side == LEFT)
		(*left_size) += 1;
	if(side == RIGHT)
		(*right_size) += 1;
	(*level) = level_current_left + 1;

	return to_return;
}

int32_t delete_node(tree *t) {
	int32_t return_val = 0;
	int32_t left_size = 0;
	int32_t right_size = 0;
	int32_t level = 0;
	node *target_node = find_parent_node(t->binary_tree, &left_size, &right_size, &level, ROOT);

	if(target_node != NULL) {
		node *to_free = NULL;
		if(target_node->right != NULL) {
			to_free = target_node->right;
			target_node->right = NULL;
		}
		else if(target_node->left != NULL) {
			to_free = target_node->left;
			target_node->left = NULL;
		}
		else {
			to_free = target_node;
			t->binary_tree = NULL;
		}

		return_val = to_free->val;

		free(to_free);
	}
	return return_val;
}

void preorder(node *n) {
	if(n == NULL)
		return;

	printf("%d\n", n->val);
	preorder(n->left);
	preorder(n->right);
}

void inorder(node *n) {
	if(n == NULL)
		return;

	inorder(n->left);
	printf("%d\n", n->val);
	inorder(n->right);
}

void postorder(node *n) {
	if(n == NULL)
		return;

	postorder(n->left);
	postorder(n->right);
	printf("%d\n", n->val);
}

void destroy_tree(node *n) {
	if(n == NULL)
		return;
	
	destroy_tree(n->left);
	destroy_tree(n->right);
	
	node *to_free;
	if(n->left != NULL) {
		to_free = n->left;
		free(to_free);
	}

	if(n->right != NULL) {
		to_free = n->right;
		free(to_free);
	}
	return;
}

void destroy(tree *t) {
	destroy_tree(t->binary_tree);
	t->binary_tree = NULL;
	free(t);
	puts("Tree Destroyed.");
	return;
}

/*node *find_node(node *n, int32_t *left_size, int32_t *right_size, int32_t *level) {
	if(n == NULL) {
		return NULL;
	}

	int32_t left_size_current = 0;
	int32_t right_size_current = 0;
	int32_t level_current_left = 0;
	int32_t level_current_right = 0;
	node *to_return = NULL;

	node *left = find_node(n->left, &left_size_current, &left_size_current, &level_current_left);
	node *right = find_node(n->right, &right_size_current, &right_size_current, &level_current_right);
	
	if(right == NULL) {
		to_return = n;
	}
	else {
		int32_t max_subtree_elem;
		if(level_current_left > level_current_right) {
			max_subtree_elem = (int32_t) ((pow(2, level_current_left + 1) - 2) / 2);
		}
		else {
			max_subtree_elem = (int32_t) ((pow(2, level_current_right + 1) - 2) / 2);
		}

		if(left_size_current < max_subtree_elem) {
			to_return = left;
		}
		else {
			to_return = right;
		}
	}

	(*left_size) += (left_size_current + 1);
	(*right_size) += (right_size_current + 1);
	(*level) = (level_current_left > level_current_right
			   ? level_current_left
			   : level_current_right) + 1;

	return NULL;
}*/

/*node *left = find_node(n->left, level+1, LEFT, t);
	node *right = find_node(n->right, level+1, RIGHT, t);

	if(left == NULL && right == NULL) {
		if(t->left > level) {
			t->right = level;
		} else {
			if(type == LEFT)
				t->left = level;
			if(type == RIGHT)
				t->right = level;
		}
		return n;
	} else if(left != NULL && right == NULL) {
		if(t->left > level) {
			t->right = level;
		} else {
			if(type == LEFT)
				t->left = level;
			if(type == RIGHT)
				t->right = level;
		}
		return n;
	}
	else {
		if(t->left == t->right) {
			return left;
		}
		if(t->left > t->right) {	
			return right;
		}
	}*/


