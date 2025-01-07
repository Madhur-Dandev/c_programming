#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "binary_tree.h"

// struct tracker tracker;

tree *init_tree(void) {
	tree *t = (tree *) malloc(sizeof(tree));
	if(t == NULL) {
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

node *find_node(node *n, int32_t *left_size, int32_t *right_size, int32_t *height, int32_t side) {
	/* recursive function for finding parent node for child 
	 * node insertion. This function find the parent node
	 * level-order wise.
	 */
	
	if(n == NULL) {
		return NULL;
	}

	int32_t left_size_current = 0;		// variable for getting size of left subtree
	int32_t right_size_current = 0;		// variable for getting size of right subtree
	int32_t height_current_left = 0;	// variable for getting height of left subtree
	int32_t height_current_right = 0;	// variable for getting height of right subtree
	node *to_return = NULL;				// address to return

	node *left = find_node(n->left, &left_size_current, &left_size_current, &height_current_left, LEFT); // recurse down left
	node *right = find_node(n->right, &right_size_current, &right_size_current, &height_current_right, RIGHT); // recurse down right
	
	if(right == NULL) {
		to_return = n;
	}
	else {
		/* It will check which side to choose based on:
		 * if the both sides have same weight then choose left,
		 * if left is less than right then left or else right
		 * vice versa of second if.
		 */
		int32_t max_subtree_elem;
		if(height_current_left > height_current_right) {
			max_subtree_elem = (int32_t) ((pow(2, height_current_left + 1) - 2) / 2);
		}
		else {
			max_subtree_elem = (int32_t) ((pow(2, height_current_right + 1) - 2) / 2);
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

	// this section is to update the value of parent variable
	// with the current child data.
	(*left_size) += left_size_current;
	(*right_size) += right_size_current;

	// this will ensure than weight is added twice by left
	// and right child
	if(side == LEFT)
		(*left_size) += 1;
	if(side == RIGHT)
		(*right_size) += 1;

	// choose greater then add 1 to original parent height
	(*height) = (height_current_left > height_current_right
			   ? height_current_left
			   : height_current_right) + 1;

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
	
	int32_t left_size, right_size, height;
	left_size = right_size = height = 0;
	node *target_node = find_node(t->binary_tree, &left_size, &right_size, &height, ROOT);
	if(height > t->height)
		t->height = height;

	if(target_node->left == NULL)
		target_node->left = n;
	else
		target_node->right = n;

	return;
}

node *find_parent_node(node *n, int32_t *left_size, int32_t *right_size, int32_t *height, int32_t side) {
	/* Same as the function use to find the parent element for
	 * insertion, but uses a differnt formula when choosing 
	 * between left or right side.
	 */
	if(n == NULL) {
		return NULL;
	}

	int32_t left_size_current = 0;
	int32_t right_size_current = 0;
	int32_t height_current_left = 0;
	int32_t height_current_right = 0;

	node *to_return = NULL;

	node *left = find_parent_node(n->left, &left_size_current, &left_size_current, &height_current_left, LEFT);
	node *right = find_parent_node(n->right, &right_size_current, &right_size_current, &height_current_right, RIGHT);
	
	if(right == NULL) {
		to_return = n;
	}
	else {
		/* if both left and right side weight is 1, then
		 * return current element
		 * if both sides are equal bu does not weigh 1, then
		 * return right
		 * if nothing works then use this formula to choose
		 * sides: (2^(max_height+1)/2) - ((2^max_height)/2)
		 * if the right side is less than or equal to the
		 * formula then choose left or else right
		 */
		if(left_size_current == 1 && right_size_current == 1) {
			to_return = n;
		}
		else if(left_size_current == right_size_current) {	
			to_return = right;
		}
		else {
			if(((((int32_t)pow(2, height_current_left + 1) / 2) - 1) - (((int32_t) pow(2, height_current_left)/2))) < right_size_current) {
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
	(*height) = height_current_left + 1;

	return to_return;
}

int32_t delete_node(tree *t) {
	int32_t return_val = 0;
	int32_t left_size = 0;
	int32_t right_size = 0;
	int32_t height = 0;
	node *target_node = find_parent_node(t->binary_tree, &left_size, &right_size, &height, ROOT);

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

	if(height < t->height)
		t->height = height;
	
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

void level_order_main(node *n, int32_t height) {
	/* This function is recurse down until the height is 1
	 * when height reaches to 1 then print the value of the
	 * current node.
	 */
	if(n == NULL)
		return;

	if(height == 1) {
		printf("%d\n", n->val);
	}
	else {
		// as goes down deeper decrement the height by 1 in each step on both the side.
		level_order_main(n->left, height - 1);
		level_order_main(n->right, height - 1);
	}
}

void level_order(tree *t) {
	// It has to invoke for each level to print.
	for(int i = 1; i <= t->height; i++) {
		level_order_main(t->binary_tree, i);
	}
	return;
}

int32_t find_smallest(node *n) {
	if(n == NULL)
		return INT_MAX;

	int32_t left = find_smallest(n->left);
	int32_t right = find_smallest(n->right);

	return left < right
		   ? (left < n->val
		  	 ? left
			 : n->val)
		   : (right < n->val
		   	  ? right
			  : n->val);
}

int32_t find_largest(node *n) {
	if(n == NULL)
		return INT_MIN;
	int32_t left = find_largest(n->left);
	int32_t right = find_largest(n->right);

	return left > right
		   ? (left > n->val
		  	 ? left
			 : n->val)
		   : (right > n->val
		   	  ? right
			  : n->val);
}

node *search_main(node *n, int32_t val) {
	if(n == NULL) {
		return NULL;
	}

	if(n->val == val) {
		return n;
	}

	node *left = search_main(n->left, val);
	node *right = search_main(n->right, val);

	return left != NULL ? left : right;
}

node *search(tree *t, int32_t val) {
	return search_main(t->binary_tree, val);
}

path_node *make_path_node(path_node *prev, int32_t side) {
	path_node *p = (path_node *) malloc(sizeof(path_node));		
	p->position = side == LEFT
				  ? 'L'
				  : side == RIGHT
				  	? 'R'
					: '-';
	p->next = prev;
	return p;

}

path_node *find_path_main(node *n, int32_t val, int32_t side) {
	if(n == NULL)
		return NULL;

	if(n->val == val) {
		return make_path_node(NULL, side);
	}
	
	path_node *left = find_path_main(n->left, val, LEFT);
	path_node *right = find_path_main(n->right, val, RIGHT);
	
	if(left != NULL) {
		return make_path_node(left, side);
	}
	else if(left == NULL && right != NULL) {
		return make_path_node(right, side);
	}
	else {
		return NULL;
	}	
}

path_node *find_path(tree *t, int32_t val) {
	return find_path_main(t->binary_tree, val, ROOT);
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


