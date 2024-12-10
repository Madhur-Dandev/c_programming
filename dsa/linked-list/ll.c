#include <stdio.h>
#include <stdlib.h>
#include "ll.h"

ll *init_list(int type) {
	// 0 for singly
	// 1 for doubly

	ll *l = (ll *) malloc(sizeof(ll));

	if(l == NULL) {
		printf("Unable to make linked list rn._.\n");
		return NULL;
	}


	switch(type) {
		case SGL:
			l->s_tail = l->s_head = NULL; 
			l->type = SGL;
			break;
		case DBL:
			l->d_tail = l->d_head = NULL; 
			l->type = DBL;
			break;
		default:
			printf("Please choose correct type\n");
			free(l);
	}

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
	void *temp;

	if(l->type == SGL) {
		temp = (s_ll *) malloc(sizeof(s_ll));
	 	((s_ll *)temp)->next = NULL;
	} else {
		temp = (d_ll *) malloc(sizeof(d_ll));
		((d_ll *)temp)->next = NULL;
	}

	if(temp == NULL) {
		puts("Unable to insertion data at this momemt.");
		return; 
	}
	
	if(l->type == SGL) {
		((s_ll *)temp)->val = val;
		if(l->s_head == NULL) {
			l->s_head = (s_ll *) temp;
			l->s_head->next = NULL;
			l->s_tail = l->s_head;
		} else {
			l->s_tail->next = (s_ll *) temp;
			l->s_tail = (s_ll *) temp;
		}
	} else {
		((d_ll *)temp)->val = val;
		if(l->d_head == NULL) {
			l->d_head = (d_ll *) temp;
			l->d_head->next = NULL;
			l->d_tail = l->d_tail;
		} else {
			((d_ll *)temp)->prev = l->d_tail;
			l->d_tail->next = (d_ll *) temp;
			l->d_tail = (d_ll *) temp;
		}
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

	if(l->type == SGL) {
		s_ll *temp = l->s_head;
		s_ll *to_free = l->s_tail;
		/*if(l->s_head == l->s_tail) {
			free(temp);
			l->s_head = l->s_tail = NULL;
			return val;
		}*/

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
		return val;
	}

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
	l->size--;
	return val;
}

int fetch(ll *l, int idx) {
	return 0;
}

int search(ll *l, int val) {
	return 0;
}

int traverse(ll *l, int flow) {
	return 0;
}

void destroy(ll *l) {
	if(l == NULL)
		return;
	void *temp;
	if(l->type == SGL) {	
		temp = l->s_head;
	} else {
		temp = l->d_head;
	}

	void *to_free;
	while(temp != NULL) {
		to_free = temp;
		if(l->type == SGL) {
			temp = ((s_ll *)temp)->next;
			free((s_ll *)to_free);
		} else {
			temp = ((d_ll *)temp)->next;
			free((d_ll *)to_free);
		}
	}

	free(l);
}
