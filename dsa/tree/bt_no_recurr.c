// fix the clear_list logic in preorder function
#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int32_t add_child(node *n, list **tail)
{
	if(n == NULL)
		return 0;

	list *new = create_list_node(n);
	if(new == NULL)
	{
		return 1;
	}

	new->prev = *tail;
	(*tail)->next = new;
	*tail = new;

	return 0;
}

void insert_node(tree *t, int32_t value) {
	node *new = build_node(value);
	if(new == NULL)
		return;

	if(t->binary_tree == NULL)
	{
		t->binary_tree = new;
	}
	else
	{
		list *head, *tail, *to_free;
		head = tail = create_list_node(t->binary_tree);
		while(head != NULL)
		{
			if(head->n->left == NULL)
			{
				head->n->left = new;
				break;
			}
			else if(head->n->right == NULL)
			{
				head->n->right = new;
				break;
			}
			else
			{
				if(add_child(head->n->left, &tail) ||
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
	if(t->binary_tree == NULL)
	{
		puts("Nothing to delete.");
		return 0;
	}

	node *parent, *to_delete;
	list *head, *tail, *to_free;
	head = tail = create_list_node(t->binary_tree);
	to_free = NULL;
	int32_t to_return;

	while(head != NULL)
	{
		if(head->n->left == NULL)
		{
		    if(to_free)
		    {
    			parent = to_free->n;
		    }
			else
		    {
				parent = head->n;
			}
			break;
		}
		else if(head->n->right == NULL)
		{
			parent = head->n;
			break;
		}
		else
		{
		    if(to_free)
		    {
				free(to_free);
				to_free = NULL;
			}
			if(add_child(head->n->left, &tail) ||
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

	if(parent->left)
    {
		to_delete = parent->right != NULL
						? parent->right
						: parent->left;

		parent->right != NULL
     	? ({parent->right = NULL;})
    	: ({parent->left = NULL;});
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
	return NULL;
}

int32_t destroy_tree(node *n)
{
	list *head, *tail, *to_free;
	head = tail = create_list_node(n);
	node *to_delete;

	while(head != NULL)
	{
		if(head->n->left)
		{
			if(add_child(head->n->left, &tail))
			{
				puts("Cannot destroy the tree.");
				return 1;
			}
		}

		if(head->n->right)
		{
			if(add_child(head->n->right, &tail))
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
	
	while(head != NULL)
	{
		printf("%d\n", head->n->value);
		
		if(add_child(head->n->left, &tail) ||
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

void preorder(node *n)
{
	list *head, *tail, *to_free, *temp;
	head = tail = create_list_node(n);
	bool is_traceback = false;

	while(head != NULL)
	{
		if(is_traceback)
		{
			if(head->n->right)
			{
				if(add_child(head->n->right, &tail))
				{
					clear_list(head);
					head = NULL;
				}
			}
			to_free = head;

			if(head->next != NULL)
				head->next->prev = head->prev;

			if(head->prev != NULL)
			{
				head->prev->next = head->next;
				head = head->prev;
			}
			else
			{
				is_traceback = false;
				head = head->next;
			}

			if(tail == to_free)
				tail = head;

			free(to_free);
		}
		else
		{
			printf("%d\n", head->n->value);
			if(head->n->left)
			{
				if(add_child(head->n->left, &tail))
				{
					clear_list(head);
					head = NULL;
				}
				head = head->next;
			}
			else
			{
				is_traceback = true;
				to_free = head;
				
				if(head->next != NULL)
					head->next->prev = head->prev;

				if(head->prev != NULL)
				{
					head->prev->next = head->next;
					head = head->prev;
				}
				else
				{
					is_traceback = false;
					head = head->next;
				}

				if(tail == to_free)
					tail = head;

				free(to_free);
			}
		}
	}

	putc('\n', stdout);
	return;
}

void inorder(node *root)
{
	list *head, *tail, *to_free;
	head = tail = create_list_node(root);
	bool is_traceback = false;

	while(head != NULL)
	{
		if(is_traceback)
		{
			printf("%d\n", tail->n->value);
			to_free = tail;
			tail = tail->prev;
			list *new = create_list_node(to_free->n->right);
			if(new == NULL)
			{
				clear_list(head);
				return;
			}

			if(tail)
			{
				tail->next = new;
				new->prev = tail;
				tail = new;
			}
			else
			{
				head = tail = new;
			}
			free(to_free);
			is_traceback = false;
		}
		else
		{
			if(tail->n->left)
			{
				if(add_child(tail->n->left, &tail))
				{
					clear_list(tail);
					return;
				}
			}
			else
			{
				printf("%d\n", tail->n->value);
				to_free = tail;
				if(tail->prev)
					tail->prev->next = NULL;
				tail = tail->prev;
				free(to_free);
				is_traceback = true;
			}
		}
	}
	return;
}
