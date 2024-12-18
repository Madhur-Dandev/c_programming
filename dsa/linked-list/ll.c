/* 1. Write comments
 * 2. Improved readability
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "ll.h"

ll *init_list(int type) {
	// 0 for singly
	// 1 for doubly

	ll *l = (ll *) malloc(sizeof(ll));

#if !defined(SGL) && !defined(DBL)
	switch(type) {
		case SGL:
			#define SGL
			l->s_tail = l->s_head = NULL; 
			break;
		case DBL:
			#define DBL
			l->d_tail = l->d_head = NULL; 
			break;
		default:
			printf("Please choose correct type\n");
			free(l);
			l = NULL;
	}
#endif

	l->size = 0;
	l->insertion = &insertion;
	l->del_rear = &del_rear;
	l->fetch = &fetch;
	l->search = &search;
	l->traverse = &traverse;
	l->destroy = &destroy;
	return l;
}

void insertion(ll *l, int val) {
#ifdef SGL
	s_ll *temp = (s_ll *) malloc(sizeof(s_ll));
	s_ll *head = l->s_head;
#elif defined(DBL)
	d_ll *temp = (d_ll *) malloc(sizeof(d_ll));
	d_ll *head = l->d_head;
#endif

	if(temp == NULL) {
		puts("Unable to insertion data at this momemt.");
		return; 
	}

#ifdef DBL
	printf("Madhur\n");
	// temp->prev = NULL;
#endif
	temp->next = NULL;
	temp->val = val;

	if(head == NULL) {
#ifdef SGL
		l->s_head = temp;
		l->s_tail = l->s_head;
#elif defined(DBL)
		l->d_head = temp;
		l->d_tail = l->d_head;
#endif
	} else {
#ifdef SGL
		l->s_tail->next = temp;
		l->s_tail = temp;
#elif defined(DBL)
		l->d_tail->next = temp;
		temp->prev = l->d_tail;
		l->d_tail = temp;
#endif
	}
	l->size++;
	puts("Data inserted.");
	return;
}

int del_rear(ll *l) {
	if(l->size <= 0) {
		printf("Nothing left to delete.\n");
		return 0;
	}
	
	int val;

#ifdef SGL
	s_ll *temp = l->s_head;
	s_ll *to_free = l->s_tail;

	while(temp->next != l->s_tail && temp->next != NULL) {
		temp = temp->next;
	}

	if(l->s_head == l->s_tail) {
		val = temp->val;
		l->s_head = l->s_tail = NULL;
	} else {
		val = to_free->val;
		temp->next = NULL;
		l->s_tail = temp;
	}
	free(to_free);
#elif defined(DBL)
	d_ll *temp = l->d_tail;
	val = temp->val;
	if(l->d_head == l->d_tail) {
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

int fetch(ll *l, int idx) {
	if(idx <= 0 || idx > l->size) {
		printf("Index is out of bound.\n");
		return 0;
	}

	int counter = idx;
	void *temp;

#ifdef SGL
	temp = l->s_head;
#elif defined(DBL)
	temp = l->d_head;
#endif

	for(int i = 1; i <= l->size; i++) {
		if(i == idx) {
#ifdef SGL
			int val = ((s_ll *)temp)->val;
#elif defined(DBL)	
			int val = ((d_ll *)temp)->val;
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

int search(ll *l, int val) {
	if(l->size <= 0) {
		printf("Nothing in the linked list.\n");
		return INT_MIN;
	}

#ifdef SGL
	s_ll *temp = l->s_head;
#elif defined(DBL)
	d_ll *temp = l->d_head;
#endif

	while(temp != NULL) {
		if(temp->val == val) {
			int val = temp->val;
			return val;
		}

		temp = temp->next;
	}

	printf("Element not found\n");
	return INT_MIN;
}

void traverse(ll *l, int flow) {
	switch(flow) {
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

void print_def(ll *l) {
#ifdef SGL
	s_ll *temp = l->s_head;
#elif defined(DBL)
	d_ll *temp = l->d_head;
#endif

	while(temp != NULL) {
		printf("%d\n", temp->val);
		temp = temp->next;
	}
	return;
}

void print_rev(ll *l) {
#ifdef DBL
	if(l->d_tail == NULL) {
		puts("List is empty.");
		return;
	}
	d_ll *temp = l->d_tail;
#elif defined(SGL)
	puts("Unable to print in reverse.");
	return;
#endif

	while(temp != NULL) {
		printf("%d\n", temp->val);
		temp = temp->prev;
	}
		
	return;
}

void destroy(ll *l) {
	if(l == NULL)
		return;

#ifdef SGL
	s_ll *temp = l->s_head;
	s_ll *to_free;
#elif defined(DBL)
	d_ll *temp = l->d_head;
	d_ll *to_free;
#endif

	while(temp != NULL) {
		to_free = temp;
		temp = temp->next;
		free(to_free);
	}

	free(l);

	puts("List destroyed.");
	return;
}
