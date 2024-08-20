/*
 * This is a method to find merging point using a stack
 * Time Complexity O(m+n)
 */

#define FORWARD_ONLY 1
#include "headers.h"
#include <inttypes.h>

typedef struct node{
	fo_node *addr;
	struct node *prev, *next;
} s_node;

int main(void) {
	fo_node *head, *head2, *temp, *temp2;
	temp = head = (fo_node *) malloc(sizeof(fo_node));
	head->data = 1;

	for(int i = 0; i < 10; i++) {
		temp2 = (fo_node *) malloc(sizeof(fo_node));
		temp2->data = i+2;
		temp->next = temp2;
		temp = temp2;
	}

	temp = temp2 = NULL;
	temp = head2 = (fo_node *) malloc(sizeof(fo_node));
	head2->data = 1;

	for(int i = 0; i < 3; i++) {
		temp2 = (fo_node *) malloc(sizeof(fo_node));
		temp2->data = i+2;
		temp->next = temp2;
		temp = temp2;
	}

	temp->next = head->next->next->next->next->next;

	temp = head;
	temp2 = head2;

	s_node *s_head, *s_head2, *s_temp, *s_temp2;

	s_head = s_head2 = s_temp = s_temp2 = NULL;

	while(1) {

		/* Building a stack for first brand */
		if(temp != NULL) {
			s_temp = (s_node *) malloc(sizeof(s_node));
			if(s_temp == NULL)
				return 1;
			s_temp->next = s_temp->prev = NULL;
			s_temp->addr = temp;
			if(s_head == NULL)
				s_head = s_temp;
			else {
				s_head->next = s_temp;
				s_temp->prev = s_head;
				s_head = s_temp;
			}

			temp = temp->next;
		}

		/* Building a stack for second brand */
		if(temp2 != NULL) {
			s_temp2 = (s_node *) malloc(sizeof(s_node));
			if(s_temp == NULL)
				return 1;
			s_temp2->next = s_temp2->prev = NULL;
			s_temp2->addr = temp2;
			if(s_head2 == NULL)
				s_head2 = s_temp2;
			else {
				s_head2->next = s_temp2;
				s_temp2->prev = s_head2;
				s_head2 = s_temp2;
			}

			temp2 = temp2->next;
		}

		if(temp == NULL && temp2 == NULL)
			break;
	}

	s_temp = s_head;
	s_temp2 = s_head2;

	while(s_temp != NULL || s_temp2 != NULL) {
		if(s_temp->addr != s_temp2->addr) {
			printf("No merging point\n");
			break;
		}

		if(s_temp->prev->addr != s_temp2->prev->addr)
			break;

		s_temp = s_temp->prev;
		s_temp2 = s_temp2->prev;
	}

//	printf("%d %d %d %d %d\n", s_temp->addr->data, head->data, head2->data , s_head->addr->data, s_head2->addr->data);

	while(1) {
		if(head) {
			temp = head;
			head = head->next;
			free(temp);
		}

		if(head2) {
			if(head2 != s_temp2->addr) {
				temp = head2;
				head2 = head2->next;
				free(temp2);
			}
			else {
				head2 = NULL;
			}
		}

		if(s_head) {
			s_temp = s_head;
			s_head = s_head->prev;
			free(s_temp);
		}

		if(s_head2) {
			s_temp = s_head2;
			s_head2 = s_head2->prev;
			free(s_temp);
		}

		if(head == NULL && head2 == NULL && s_head == NULL && s_head2 == NULL)
			break;
	}
	s_head = s_head2 = s_temp = s_temp2 = NULL;
	head = head2 = temp = temp2 = NULL;

	return 0;
}
