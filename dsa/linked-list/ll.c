/* 1. Write comments
 * 2. Declare and define the remaining functions.
 * 3. Return an sturcture instead of number in
 *    delete, fetch, search operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include "ll.h"

#if defined(SGL) || defined(DBL)

ll *init_list()
{
	ll *l = (ll *)malloc(sizeof(ll));

#ifdef SGL
	l->s_tail = l->s_head = NULL;
#elif defined(DBL)
	l->d_tail = l->d_head = NULL;
#else
	printf("Please choose correct type\n");
	free(l);
	return NULL;
#endif

	// initializing the variable fields for the list
	l->size = 0;
	l->insert_at_front = &insert_at_front;
	l->insert_at_index = &insert_at_index;
	l->insert_at_end = &insert_at_end;
	l->delete_from_front = &delete_from_front;
	l->delete_from_index = &delete_from_index;
	l->delete_from_end = &delete_from_end;
	l->fetch = &fetch;
	l->search = &search;
	l->print_list = &print_list;
	l->destroy = &destroy;
	return l;
}


// insertion section - contains all the functions
// for insertion. These function do not return anything.
// ------------------------Start------------------------
void insert_at_front(ll *l, int32_t val)
{
	/* First initialize the temp and head variable.
	 * temp - for newly created node of list.
	 * head - for checking the head of either 
	 *		  singly or doubly is null or not.
	 */
#ifdef SGL
	s_ll *temp = (s_ll *)malloc(sizeof(s_ll));
	s_ll *head = l->s_head;
#elif defined(DBL)
	d_ll *temp = (d_ll *)malloc(sizeof(d_ll));
	d_ll *head = l->d_head;
#endif

	if (temp == NULL)
	{
		// Check if the memory allocation of new node fails.
		puts("Unable to insertion data at this momemt.");
		return;
	}

// main logic for the insertion at front.
#ifdef DBL
	temp->prev = NULL;
#endif
	temp->next = NULL;
	temp->val = val;

	if (head == NULL)
	{
#ifdef SGL
		l->s_head = temp;
		l->s_tail = l->s_head;
#elif defined(DBL)
		l->d_head = temp;
		l->d_tail = l->d_head;
#endif
	}
	else
	{
#ifdef SGL
		temp->next = l->s_head;
		l->s_head = temp;
#elif defined(DBL)
		temp->next = l->d_head;
		l->d_head->prev = temp;
		l->d_head = temp;
#endif
	}
	l->size++;
	puts("Data inserted.");
	return;
}

void insert_at_index(ll *l, int32_t val, int32_t index)
{
	/* 1. first if for verifying the given index
	 * 2. second if for checking if the index is 0
	 *    if it is 0 then call the insert_at_front.
	 * 3. third if for checking if the index is last
	 *    index value, if it is then call insert_at_end
	 * 
	 * Additional logic for inserting at given index
	 * requires to traverse to the index - 1 node and
	 * then adjusting the pointers depending on the
	 * type of linked-list and adding new node.
	 */
	if (index < 0 || index >= l->size)
	{
		puts("Invalid index.");
		return;
	}

	if (index == 0)
	{
		insert_at_front(l, val);
		return;
	}

	if (index == l->size - 1)
	{
		insert_at_end(l, val);
		return;
	}

#ifdef SGL
	s_ll *temp = (s_ll *)malloc(sizeof(s_ll));
	s_ll *head = l->s_head;
#elif defined(DBL)
	d_ll *temp = (d_ll *)malloc(sizeof(d_ll));
	d_ll *head = l->d_head;
#endif

	if (temp == NULL)
	{
		// Check if the memory allocation of new node fails.
		puts("Unable to insertion data at this momemt.");
		return;
	}

#ifdef DBL
	temp->prev = NULL;
#endif
	temp->next = NULL;
	temp->val = val;

	int32_t counter = 0;
	while (head != NULL)
	{
		if (counter == index - 1)
			break;

		head = head->next;
		counter++;
	}

#ifdef SGL
	temp->next = head->next;
	head->next = temp;
#elif defined(DBL)
	temp->next = head->next;
	temp->prev = head;
	head->next->prev = temp;
	head->next = temp;
#endif
	l->size++;
	puts("Data inserted.");
	return;
}

void insert_at_end(ll *l, int32_t val)
{
	/* The insert_at_end function is straight forward.
	 * It doesn't require additional traversing logic
	 * just some adjustment of pointers of tail node.
	 */
	if (l->size <= 0)
	{
		insert_at_front(l, val);
		return;
	}

#ifdef SGL
	s_ll *temp = (s_ll *)malloc(sizeof(s_ll));
	s_ll *head = l->s_head;
#elif defined(DBL)
	d_ll *temp = (d_ll *)malloc(sizeof(d_ll));
	d_ll *head = l->d_head;
#endif

	if (temp == NULL)
	{
		puts("Unable to insertion data at this momemt.");
		return;
	}

#ifdef DBL
	temp->prev = NULL;
#endif
	temp->next = NULL;
	temp->val = val;

#ifdef SGL
	l->s_tail->next = temp;
	l->s_tail = temp;
#elif defined(DBL)
	l->d_tail->next = temp;
	temp->prev = l->d_tail;
	l->d_tail = temp;
#endif

	l->size++;
	puts("Data inserted.");
	return;
}
// -------------------------End--------------------------



// deletion section - contains all the functions
// for deletion. These function return the value
// contained by the deleted node.
// ------------------------Start------------------------
int32_t delete_from_front(ll *l)
{
	/* Fairly simple logic:
	 * Just adjust the pointer of next and
	 * and prev(only doubly ll's node) and
	 * free the head node and is next pointer
	 * is null then assign both head and tail
	 * to null.
	 * Using to_free variable to hold the
	 * address of the head node.
	 */
	if (l->size <= 0)
	{
		// check if list is null or not.
		printf("Nothing left to delete.\n");
		return 0;
	}

	int32_t val;
#ifdef SGL
	s_ll *to_free = l->s_head;
	l->s_head = l->s_head->next;
	if(l->s_head == NULL) {
		l->s_head = l->s_tail = NULL;
	}
#elif defined(DBL)
	d_ll *to_free = l->d_head;
	l->d_head = l->d_head->next;
	if(l->d_head == NULL) {
		l->d_head = l->d_tail = NULL;
	} else {
		l->d_head->prev = NULL;
	}
#endif
	val = to_free->val;
	free(to_free);
	l->size--;
	return val;
}

int32_t delete_from_index(ll *l, int32_t index)
{
	/* Traversing upto index - 1 and then adjusting
	 * the pointers and freeing the index node.
	 */
	if (l->size < 0 || index >= l->size)
	{
		puts("Invalid index.\n");
		return 0;
	}
	if (index == 0)
	{
		return delete_from_front(l);
	}
	if (index == l->size - 1)
	{
		return delete_from_end(l);
	}

#ifdef SGL
	s_ll *head = l->s_head;
#elif defined(DBL)
	d_ll *head = l->d_head;
#endif

	int32_t counter = 0;
	while (head != NULL)
	{
		if (counter == index - 1)
			break;

		head = head->next;
		counter++;
	}

	int32_t val;
#ifdef SGL
	s_ll *to_free = head->next;
	head->next = head->next->next;
#elif defined(DBL)
	d_ll *to_free = head->next;
	head->next->next->prev = head;
	head->next = head->next->next;
#endif
	val = to_free->val;
	free(to_free);
	l->size--;
	puts("Data deleted.");
	return val;
}

int32_t delete_from_end(ll *l)
{
	/* The delete_from_end function doesn't require
	 * additions traversing logic for doubly
	 * linked-list but requires the logic for
	 * singly linked-list. The remainder of code
	 * is almost similar as function insert_at_front.
	 */

	if (l->size <= 0)
	{
		printf("Nothing left to delete.\n");
		return 0;
	}

	if(l->size <= 1) {
		return delete_from_front(l);
	}

	int32_t val;
#ifdef SGL
	s_ll *to_free;
	s_ll *head = l->s_head;
	int32_t counter = 1;
	while(head != NULL) {
		if(counter == l->size-1)
			break;
		head = head->next;
		counter++;
	}

	to_free = head->next;
	head->next = NULL;
	l->s_tail = head;
#elif defined(DBL)
	d_ll *to_free = l->d_tail;
	l->d_tail = l->d_tail->prev;
	l->d_tail->next = NULL;
#endif
	val = to_free->val;
	free(to_free);
	l->size--;
	return val;
}
// -------------------------End-------------------------


int32_t fetch(ll *l, int32_t index)
{
	/* fetch function fetch the value hold by
	 * node at index position.
	 */
	if (index < 0 || index >= l->size)
	{
		// Check if the index given is valid or not.
		printf("Index is out of bound.\n");
		return 0;
	}


// node variable for temporarily holding the address
// of nodes as we progresses in loop and which will
// also hold the address of node at index position.
#ifdef SGL
	s_ll *node = l->s_head;
#elif defined(DBL)
	d_ll *node = l->d_head;
#endif

	if(index == l->size - 1)
	{
#ifdef SGL
		node = l->s_tail;		
#elif defined(DBL)
		node = l->d_tail;
#endif
	}

	if(index > 0 && index < l->size - 1)
	{
		int32_t counter = 0;	// for counting position as we progresses in loop.
		while(node != NULL)
		{
			if (counter == index)
			{
				break;	
			}
			node = node->next;
			counter++;
		}
	}

	return node->val;
}

int32_t search(ll *l, int32_t val)
{
	if (l->size <= 0)
	{
		printf("Nothing in the linked list.\n");
		return INT_MIN;
	}

#ifdef SGL
	s_ll *temp = l->s_head;
#elif defined(DBL)
	d_ll *temp = l->d_head;
#endif

	int32_t counter = 0;
	while (temp != NULL)
	{
		if (temp->val == val)
		{
			return counter;
		}

		temp = temp->next;
		counter++;
	}

	printf("Element not found\n");
	return -1;
}

void print_list(ll *l, int32_t flow)
{
	switch (flow)
	{
	case DEF:
		print_def(l);
		break;
	case REV:
		print_rev(l);
		break;
	default:
		return;
	}
	return;
}

void print_singly_def(s_ll *node) {
	if(node != NULL) {
		print_singly_def(node->next);
		printf("%d\n", node->val);
	}
	return;
}

void print_def(ll *l)
{
#ifdef SGL
	s_ll *temp = l->s_head;
#elif defined(DBL)
	d_ll *temp = l->d_head;
#endif
	while (temp != NULL)
	{
		printf("%d\n", temp->val);
		temp = temp->next;
	}
	puts("");
	return;
}

void print_rev(ll *l)
{
#ifdef SGL
	print_singly_def(l->s_head);
#elif defined(DBL)
	if (l->d_tail == NULL)
	{
		puts("List is empty.");
		return;
	}
	d_ll *temp = l->d_tail;
	while (temp != NULL)
	{
		printf("%d\n", temp->val);
		temp = temp->prev;
	}
#endif
	puts("");
	return;
}

void destroy(ll *l)
{
	if (l == NULL)
		return;

#ifdef SGL
	s_ll *temp = l->s_head;
	s_ll *to_free;
#undef SGL
#elif defined(DBL)
	d_ll *temp = l->d_head;
	d_ll *to_free;
#undef DBL
#endif

	while (temp != NULL)
	{
		to_free = temp;
		temp = temp->next;
		free(to_free);
	}

	free(l);

	puts("List destroyed.");
	return;
}
#endif
