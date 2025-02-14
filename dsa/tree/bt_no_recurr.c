// fix linking error due to mismatch function name:
// search -> search_main
// find_largest -> find_largest_main
// find_smallest -> find_smallest_main

#include "binary_tree.h"
#include "helper_function.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int32_t add_child(node *n, list **tail)
{
	/* Utility function for adding child
	 * in dynamic queue.
	 */
	if (n == NULL)
		return 0;

	list *new = create_list_node(n);
	if (new == NULL)
	{
		return 1;
	}

	new->prev = *tail;
	if (*tail == NULL)
	{
		*tail = new;
		new->next = NULL;
	}
	else
	{
		(*tail)->next = new;
		*tail = new;
	}
	return 0;
}

node *find_parent(node *root)
{
	/* This function finds parent for insertion
	 * Logic:
	 * 1. each node of tree is added to queue.
	 * 2. The nodes will be check for following:
	 *    i. if missing both or any one of child 
	 		 then return that node.
	 *   ii. else add both left and right child
	 		 to the queue.
	 */
	list *head, *tail, *to_free;
	head = tail = create_list_node(root);
	node *parent = NULL;
	while (head != NULL)
	{
		if (!(head->n->left) || !(head->n->right))
		{
			parent = head->n;
			clear_list(head);
			return parent;
		}
		else
		{
			if (add_child(head->n->left, &tail) ||
				add_child(head->n->right, &tail))
			{
				clear_list(head);
				puts("Cannot insert the value in tree.");
				return NULL;
			}

			to_free = head;
			head = head->next;
			free(to_free);
		}
	}
	return NULL;
}

node *handle_deletion(node **root)
{
	/* Node from the root of the tree will be added
	 * to the queue (level-order wise).
	 * following condition will be checked:
	 * 1. if no left child for current element then
	 	  then return preceeding element, if available
		  else return current element.
	 * 2. if only left is present return current.
	 * 3. else add subsequent childs to the queue.
	 */
	if (*root == NULL)
		return NULL;

	node *parent, *to_delete;
	list *head, *tail, *to_free;
	head = tail = create_list_node(*root);
	to_free = NULL;

	while (head != NULL)
	{
		if (head->n->left == NULL)
		{
			if (to_free)
			{
				parent = to_free->n;
			}
			else
			{
				parent = head->n;
			}
			break;
		}
		else if (head->n->right == NULL)
		{
			parent = head->n;
			break;
		}
		else
		{
			if (to_free)
			{
				free(to_free);
				to_free = NULL;
			}
			if (add_child(head->n->left, &tail) ||
				add_child(head->n->right, &tail))
			{
				clear_list(head);
				puts("Cannot delete from tree.");
				return 0;
			}

			to_free = head;
			head = head->next;
		}
	}

	free(to_free);
	clear_list(head);

	if (parent->left)
	{
		to_delete = parent->right != NULL
						? parent->right
						: parent->left;

		parent->right != NULL
			? ({ parent->right = NULL; })
			: ({ parent->left = NULL; });
	}
	else
	{
		to_delete = parent;
	 	*root = NULL;
	}

	return to_delete;
}

int32_t get_height_main(node *root)
{
	/* Since the tree is filled level-order wise
	 * We'll only check high from root to the left-most
	 * element in the tree.
	 */
	int32_t height = 0;

	list *head, *tail, *to_free;
	head = tail = create_list_node(root);

	while(head)
	{
		height++;
		to_free = head;
		if(add_child(head->n->left, &tail))
		{
			clear_list(head);
			puts("Cannot fetch height");
			return 0;
		}
		head = head->next;
		free(to_free);
	}
	
	return height;
}

int32_t get_node_level_main(node *root, int32_t target)
{	
	/* Start with level 0
	 * return the level if node value matches the target
	 * else reduce teil(total elements in level which is
	 * calculated by 2^level
	 * Since we are dealing with level-order wise checking
	 * we need to reduce teil by 1 everytime we check a
	 * node. When teil come back to 0, we increase level
	 * and re-calculate teil
	 * Repeat this until we found our node.
	 * If no node found with target value return or any
	 * error occured return -1.
	 */
	int32_t level = 0;
	int32_t total_element_in_level = (int32_t) pow(2, (int32_t) level);
	int32_t *teil = &total_element_in_level;

	list *head, *tail, *to_free;
	head = tail = create_list_node(root);

	while(head)
	{
		if(head->n->value == target)
		{
			return level;
		}

		(*teil)--;
		if(*teil == 0)
		{
			level++;
			*teil = pow(2, (int32_t) level);
		}

		if(head->n->left)
		{
			if(add_child(head->n->left, &tail))
			{
				clear_list(head);
				return -1;
			}
		}
		else
		{
			(*teil)--;
		}

		if(head->n->right)
		{
			if(add_child(head->n->right, &tail))
			{
				clear_list(head);
				return -1;
			}
		}
		else
		{
			(*teil)--;
		}

		to_free = head;
		head = head->next;
		free(to_free);
	}

	return -1;
}

void preorder_main(node *root)
{
	/* Make a stack and insert root node.
	 * Print the value of current node.
	 * Then if left node is present, add it to stack
	 * else pop the current node from stack and
	 * traceback to previous node in stack
	 * pop the currently pointed node
	 * if poped node have right node then add it to
	 * stack
	 * else traceback to previous node
	 * do it until stack get empty and no nodes from
	 * tree are remained visited.
	 */
	list *head, *tail, *to_free, *temp;
	head = tail = create_list_node(root);
	bool is_traceback = false;

	while(tail != NULL)
	{
		if(is_traceback)
		{
			if(tail->n->right == to_free->n)
			{
				if(to_free)
					free(to_free);

				to_free = tail;
				tail = tail->prev;
			}
			else
			{
				if(to_free)
					free(to_free);

				if(add_child(tail->n->right, &tail))
				{
					clear_list(head);
					return;
				}
				is_traceback = false;
			}
		}
		else
		{
			printf("%d\n", tail->n->value);
			if(tail->n->left != NULL)
			{
				if(add_child(tail->n->left, &tail))
				{
					clear_list(head);
					return;
				}
			}
			else
			{
				is_traceback = true;
				to_free = tail;
				tail = tail->prev;
			}
		}
	}

	return;
}

void inorder_main(node *root)
{
	/* Make a stack and insert the root
	 * Add the left node if available
	 * else print the value of current node
	 * pop that node and traceback
	 * pop the tracebacked node and check if
	 * it has right child. If yes, add it to
	 * stack.
	 * Repeat this until stack is empty
	 */
	list *head, *tail, *to_free;
	head = tail = create_list_node(root);
	to_free = NULL;
	bool is_traceback = false;

	while (tail != NULL)
	{
		if (to_free)
		{
			printf("%d\n", to_free->n->value);
			free(to_free);
		}
		if (is_traceback)
		{
			to_free = tail;
			tail = tail->prev;
			if (add_child(to_free->n->right, &tail))
			{
				clear_list(head);
				return;
			}
			is_traceback = false;
		}
		else
		{
			if (tail->n->left != NULL)
			{
				if (add_child(tail->n->left, &tail))
				{
					clear_list(head);
					return;
				}
				to_free = NULL;
			}
			else
			{
				to_free = tail;
				tail = tail->prev;
				is_traceback = true;
			}
		}
	}

	if (to_free != NULL)
	{
		printf("%d\n", to_free->n->value);
		free(to_free);
	}

	return;
}

void postorder_main(node *root)
{
	/* Make a stack and add root node
	 * If left element is available
	 * then add to it to stack
	 * else print the value of current element
	 * pop the node an traceback
	 * check if tracebacked node have right node
	 * if yes then add to stack
	 * else print the value of tracebacked node and pop the tracebacked node
	 * do it until stack gets empty
	 */
	list *head, *tail, *to_free;
	bool is_traceback = false;

	head = tail = create_list_node(root);
	to_free = NULL;

	while (tail != NULL)
	{
		if (is_traceback)
		{
			printf("%d\n", to_free->n->value);
			if (to_free->n == tail->n->right)
			{
				free(to_free);
				to_free = tail;
				tail = tail->prev;
			}
			else
			{
				free(to_free);
				if (add_child(tail->n->right, &tail))
				{
					clear_list(head);
					return;
				}

				is_traceback = false;
			}
		}
		else
		{
			to_free = tail;
			if (tail->n->left != NULL)
			{
				if (add_child(tail->n->left, &tail))
				{
					clear_list(head);
					return;
				}
			}
			else
			{
				is_traceback = true;
				tail = tail->prev;
			}
		}
	}

	if (to_free != NULL)
	{
		printf("%d\n", to_free->n->value);
		free(to_free);
	}
	
	return;
}

void level_order_main(node *root)
{
	/* Make a stack and add the root
	 * print the value of current node
	 * add the left and right node if available
	 * pop the current node
	 * do it until stack get empty
	 */
	list *head, *tail, *to_free;
	head = tail = create_list_node(root);

	while (head != NULL)
	{
		printf("%d\n", head->n->value);

		if (add_child(head->n->left, &tail) ||
			add_child(head->n->right, &tail))
		{
			clear_list(head);
			puts("Unable to perform level-order traversal.");
			return;
		}

		to_free = head;
		head = head->next;
		free(to_free);
	}

	return;
}

void level_order(tree *t)
{
	puts("Level-order");
	level_order_main(t->binary_tree);
	putc('\n', stdout);
}

int32_t find_smallest_main(node *root)
{
	/* Make a stack and insert the root
	 * While the logic is similar to leve-order traversal
	 * We just some step:
	 * compare value as we advance and stay updated with
	 * smallest value.
	 */
	list *head, *tail, *to_free;
	head = tail = create_list_node(root);
	to_free = NULL;
	if(head == NULL)
	{
		puts("Unable to search.");
		return INT_MIN;
	}

	int32_t result = head->n->value;
	while(head)
	{
		if(result > head->n->value)
			result = head->n->value;

		if(add_child(head->n->left, &tail) ||
			add_child(head->n->right, &tail))
		{
			clear_list(head);
			puts("Unable to search.");
			return INT_MIN;
		}
		if(to_free)
			free(to_free);

		to_free = head;
		head = head->next;
	}
	
	if(to_free)
		free(head);
	return result;
}

int32_t find_largest_main(node *root)
{
	/* Make a stack and insert the root
	 * While the logic is similar to leve-order traversal
	 * We just some step:
	 * compare value as we advance and stay updated with
	 * largest value.
	 */
	list *head, *tail, *to_free;
	head = tail = create_list_node(root);
	to_free = NULL;
	if(head == NULL)
	{
		puts("Unable to search.");
		return INT_MIN;
	}

	int32_t result = head->n->value;
	while(head)
	{
		if(result < head->n->value)
			result = head->n->value;

		if(add_child(head->n->left, &tail) ||
			add_child(head->n->right, &tail))
		{
			clear_list(head);
			puts("Unable to search.");
			return INT_MIN;
		}
		if(to_free)
			free(to_free);

		to_free = head;
		head = head->next;
	}
	
	if(to_free)
		free(head);
	return result;
}

node *search_main(node *root, int32_t value)
{
	list *head, *tail, *to_free;
	head = tail = create_list_node(root);
	to_free = NULL;
	node *target = NULL;
	if(head == NULL)
	{
		puts("Unable to search.");
		return target;
	}

	while(head)
	{
		if(head->n->value == value)
		{
			target = head->n;
			clear_list(head);
			return target;
		}
		
		if(add_child(head->n->left, &tail) ||
			add_child(head->n->right, &tail))
		{
			clear_list(head);
			puts("Unable to search.");
			return target;
		}
		if(to_free)
			free(to_free);

		to_free = head;
		head = head->next;
	}
	if(to_free)
		free(head);
	return target;
}

path_node *find_path(tree *t, int32_t value)
{
	list *head, *tail, *to_free;
	bool is_traceback = false;
	path_node *path_head, *path_tail, *path_temp;
	path_head = path_tail = path_temp = NULL;
	head = tail = create_list_node(t->binary_tree);
	to_free = NULL;

	while (tail != NULL)
	{	
		if (is_traceback)
		{
			if(tail->n->right == to_free->n)
			{
				free(to_free);
				to_free = tail;
				tail = tail->prev;
			}
			else
			{
				free(to_free);
				if (add_child(tail->n->right, &tail))
				{
					clear_list(head);
					return NULL;
				}
				to_free = NULL;
				is_traceback = false;
			}
		}
		else
		{
			if(tail->n->value == value)
				break;
		
			if (tail->n->left != NULL)
			{
				if (add_child(tail->n->left, &tail))
				{
					clear_list(head);
					return NULL;
				}
			}
			else
			{
				is_traceback = true;
				to_free = tail;
				tail = tail->prev;
			}
		}
	}
	
	if(to_free == head)
		head = NULL;

	if(to_free)
		free(to_free);

	to_free = NULL;

	while(head != NULL)
	{
		if(to_free == NULL)
		{
			path_head = path_tail = make_path_node(NULL, ROOT);
			if(path_head == NULL)
			{
				clear_list(head);
				return NULL;
			}
		}
		else
		{
			path_temp = make_path_node(NULL, to_free->n->right == head->n
											 ? RIGHT : LEFT);
			path_tail->next = path_temp;
			path_tail = path_temp;
			free(to_free);

		}
		to_free = head;
		head = head->next;
	}
	return path_head;
}

int32_t destroy_tree(node *n)
{
	list *head, *tail, *to_free;
	head = tail = create_list_node(n);
	node *to_delete;

	while (head != NULL)
	{
		if (head->n->left)
		{
			if (add_child(head->n->left, &tail))
			{
				puts("Cannot destroy the tree.");
				return 1;
			}
		}

		if (head->n->right)
		{
			if (add_child(head->n->right, &tail))
			{
				puts("Cannot destroy the tree.");
				return 1;
			}
		}

		to_delete = head->n;
		free(to_delete);
		to_free = head;
		head = head->next;
		free(to_free);
	}

	return 0;
}
