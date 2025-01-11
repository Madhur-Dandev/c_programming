#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>

int32_t add_child(node *n, list *head, list *tail)
{
	/*list *next1 = create_list_node(n->left);
	list *next2 = create_list_node(n->right);*/

	if(n == NULL)
		return 0;

	list *new = create_list_node(n);
	if(new == NULL)
	{
		clear_list(head);
		return 1;
	}
			
	/*if(next1 == NULL || next2 == NULL)
	{
		if(next1 != NULL)
			free(next1);

		if(next2 != NULL)
			free(next2);

		clear_list(head);
		puts("Cannot insert the value in tree.");
		return 1;
	}*/

	new->prev = tail;
	tail->next = new;
	tail = new;

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
				/*tree *next1 = create_list_node(head->n->left);
				tree *next2 = create_list_node(head->n->right);
				
				if(next1 == NULL || next2 == NULL)
				{
					if(next1 != NULL)
						free(next1);

					if(next2 != NULL)
						free(next2);

					clear_list(head);
					puts("Cannot insert the value in tree.");
					return;
				}

				next1->next = next2;
				tail->next = next1;
				tail = next2;*/
				
				if(add_child(head->n->left, head, tail))
				{
					puts("Cannot insert the value in tree.");
					return;
				}

				if(add_child(head->n->right, head, tail))
				{
					puts("Cannot insert the value in tree.");
					return;
				}
				
				to_free = head;
				head = head->next;
				free(to_free);
			}
		}

		puts("Value inserted in tree");
		return;
	}
}

int32_t delete_node(tree *t)
{
	if(t->binary_tree == NULL)
	{
		puts("Nothing to delete.");
		return 0;
	}

	node *parent, *to_delete;
	list *head, *tail, *temp, *to_free;
	head = tail = temp = create_list_node(t->binary_tree);
	int32_t to_return;

	while(temp != NULL)
	{
		if(head->n->left == NULL)
		{
			parent = head->n;
			break;
		}
		else if(head->n->right == NULL)
		{
			parent = temp->n;
			break;
		}
		else
		{
			/*tree *next1 = create_list_node(head->n->left);
			tree *next2 = create_list_node(head->n->right);
				
			if(next1 == NULL || next2 == NULL)
			{
				if(next1 != NULL)
					free(next1);

				if(next2 != NULL)
					free(next2);

				clear_list(head);
				puts("Cannot insert the value in tree.");
				return;
			}

			next1->prev = tail;
			next2->prev = next1;
			next1->next = next2;
			tail->next = next1;
			tail = next2;*/
			if(add_child(temp->n->left, head, tail))
			{
				puts("Cannot delete from tree.");
				return 0;
			}

			if(add_child(temp->n->right, head, tail))
			{
				puts("Cannot delete from tree.");
				return 0;
			}	

			to_free = head;
			free(to_free);
			head = temp;
			
			temp = temp->next;
		}
	}

	clear_list(head);

	to_delete = parent->right != NULL
				? parent->right
				: parent->left;

	to_return = to_delete->value;

	parent->right != NULL
	? ({parent->right = NULL;})
	: ({parent->right = NULL;});
	
	free(to_delete);
	return to_return;
}

path_node *find_path(tree *t, int32_t value)
{
	return NULL;
}

void destroy_tree(node *n)
{
	list *head, *tail, *to_free;
	head = tail = create_list_node(n);
	node *to_delete;

	while(head != NULL)
	{
		if(head->n->left)
		{
			if(add_child(head->n->left, head, tail))
			{
				puts("Cannot destroy the tree.");
				return;
			}
		}

		if(head->n->right)
		{
			if(add_child(head->n->right, head, tail))
			{
				puts("Cannot destroy the tree.");
				return;
			}
		}

		to_delete = head->n;
		free(to_delete);
		to_free = head;
		head = head->next;
		free(to_free);
	}
}
