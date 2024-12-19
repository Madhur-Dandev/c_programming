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

ll *init_list(int32_t type)
{
	// 0 for singly
	// 1 for doubly

	ll *l = (ll *)malloc(sizeof(ll));

	switch (type)
	{
	case 0:
		l->s_tail = l->s_head = NULL;
		break;
	case 1:
		l->d_tail = l->d_head = NULL;
		break;
	default:
		printf("Please choose correct type\n");
		free(l);
		l = NULL;
	}

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

void insert_at_front(ll *l, int32_t val)
{
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
	// temp->prev = NULL;
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
	if (index < 0 || index <= l->size)
	{
		puts("Invalid index.\n");
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
		puts("Unable to insertion data at this momemt.");
		return;
	}

#ifdef DBL
	// temp->prev = NULL;
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
	puts("Data inserted");
	return;
}

void insert_at_end(ll *l, int32_t val)
{
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
	puts("Madhur2.");
	// temp->prev = NULL;
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
	puts("Data inserted");
	return;
}

int32_t delete_from_front(ll *l)
{
	if (l->size <= 0)
	{
		printf("Nothing left to delete.\n");
		return 0;
	}

	int val;
#ifdef SGL
	s_ll *to_free = l->s_head;
	l->s_head = l->s_head->next;
#elif defined(DBL)
	d_ll *to_free = l->d_head;
	l->d_head = l->d_head->next;
	l->d_head->prev = NULL;
#endif
	val = to_free->val;
	free(to_free);
	l->size--;
	return val;
}

int32_t delete_from_index(ll *l, int32_t index)
{
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

	int val;
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
	if (l->size <= 0)
	{
		printf("Nothing left to delete.\n");
		return 0;
	}

	int32_t val;

#ifdef SGL
	s_ll *temp = l->s_head;
	s_ll *to_free = l->s_tail;

	while (temp->next != l->s_tail && temp->next != NULL)
	{
		temp = temp->next;
	}

	if (l->s_head == l->s_tail)
	{
		val = temp->val;
		l->s_head = l->s_tail = NULL;
	}
	else
	{
		val = to_free->val;
		temp->next = NULL;
		l->s_tail = temp;
	}
	free(to_free);
#elif defined(DBL)
	d_ll *temp = l->d_tail;
	val = temp->val;
	if (l->d_head == l->d_tail)
	{
		free(temp);
		l->d_head = l->d_tail = NULL;
		return val;
	}

	d_ll *to_free = temp;
	l->d_tail = temp->prev;
	l->d_tail->next = NULL;
	free(to_free);
#endif
	l->size--;
	return val;
}

int32_t fetch(ll *l, int32_t idx)
{
	if (idx <= 0 || idx > l->size)
	{
		printf("Index is out of bound.\n");
		return 0;
	}

	int32_t counter = idx;
	void *temp;

#ifdef SGL
	temp = l->s_head;
#elif defined(DBL)
	temp = l->d_head;
#endif

	for (int32_t i = 1; i <= l->size; i++)
	{
		if (i == idx)
		{
#ifdef SGL
			int32_t val = ((s_ll *)temp)->val;
#elif defined(DBL)
			int32_t val = ((d_ll *)temp)->val;
#endif
			return val;
		}

#ifdef SGL
		temp = ((s_ll *)temp)->next;
#elif defined(DBL)
		temp = ((d_ll *)temp)->next;
#endif
	}

	return INT_MIN;
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

	while (temp != NULL)
	{
		if (temp->val == val)
		{
			int32_t val = temp->val;
			return val;
		}

		temp = temp->next;
	}

	printf("Element not found\n");
	return INT_MIN;
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
	return;
}

void print_rev(ll *l)
{
#ifdef DBL
	if (l->d_tail == NULL)
	{
		puts("List is empty.");
		return;
	}
	d_ll *temp = l->d_tail;
#elif defined(SGL)
	puts("Unable to print in reverse.");
	return;
#endif
	/*
		while (temp != NULL)
		{
			printf("%d\n", temp->val);
			temp = temp->prev;
		}
	*/
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
