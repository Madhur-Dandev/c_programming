#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int32_t add_child(node *n, list **tail)
{
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

void insert_node(tree *t, int32_t value)
{
	node *new = build_node(value);
	if (new == NULL)
		return;

	if (t->binary_tree == NULL)
	{
		t->binary_tree = new;
	}
	else
	{
		list *head, *tail, *to_free;
		head = tail = create_list_node(t->binary_tree);
		while (head != NULL)
		{
			if (head->n->left == NULL)
			{
				head->n->left = new;
				break;
			}
			else if (head->n->right == NULL)
			{
				head->n->right = new;
				break;
			}
			else
			{
				if (add_child(head->n->left, &tail) ||
					add_child(head->n->right, &tail))
				{
					clear_list(head);
					puts("Cannot insert the value in tree.");
					return;
				}

				to_free = head;
				head = head->next;
				free(to_free);
			}
		}
	}
	puts("Value inserted in tree");
	return;
}

int32_t delete_node(tree *t)
{
	if (t->binary_tree == NULL)
	{
		puts("Nothing to delete.");
		return 0;
	}

	node *parent, *to_delete;
	list *head, *tail, *to_free;
	head = tail = create_list_node(t->binary_tree);
	to_free = NULL;
	int32_t to_return;

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
		t->binary_tree = NULL;
	}

	to_return = to_delete->value;
	free(to_delete);
	return to_return;
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

void level_order(tree *t)
{
	list *head, *tail, *to_free;
	head = tail = create_list_node(t->binary_tree);

	printf("Level-Order:\n");
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
	putc('\n', stdout);
}

void preorder(tree *t)
{
	list *head, *tail, *to_free, *temp;
	head = tail = create_list_node(t->binary_tree);
	bool is_traceback = false;

	printf("Pre-Order:\n");
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

	putc('\n', stdout);
	return;
}

void inorder(tree *t)
{
	list *head, *tail, *to_free;
	head = tail = create_list_node(t->binary_tree);
	to_free = NULL;
	bool is_traceback = false;

	printf("In-Order:\n");
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
	puts("");
	return;
}

void postorder(tree *t)
{
	list *head, *tail, *to_free;
	bool is_traceback = false;

	head = tail = create_list_node(t->binary_tree);
	to_free = NULL;

	printf("Post-Order:\n");
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
	puts("");
	return;
}
