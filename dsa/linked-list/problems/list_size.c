#define FORWARD_ONLY 1
#include "headers.h"

int main(void) {
	fo_node *head, *tail, *temp;
	tail = head = (fo_node *) malloc(sizeof(fo_node));
	head->data = 1;

	for(int i = 0; i < 100; i++) {
		temp = (fo_node *) malloc(sizeof(fo_node));
		temp->data = i+2;
		tail->next = temp;
		tail = temp;
	}

	tail->next = head->next->next->next;

	fo_node *slow, *fast;
	slow = fast = head;

	temp = NULL;

	while(1) {
		slow = slow->next;
		if(temp == NULL) {
			if(fast == NULL)
				break;
			fast = fast->next;

			if(fast == NULL)
				break;
			fast = fast->next;

			if(slow == fast) {
				temp = slow;
				slow = head;
			}
		}
		else {
			if(slow == fast->next) {
				temp = fast;
				break;
			}
			fast = fast->next;
		}
	}

	printf("%p %p %p\n", temp, temp->next, head->next->next->next->next);
	printf("%d %d\n", temp->data, temp->next->data);

	int counter = 0;
	tail = head;

	while(1) {
		counter++;
		if((temp != NULL &&
			temp == tail) ||
			tail->next == NULL)
			break;
		tail = tail->next;
	}

	printf("%d\n", counter);
	return 0;
}
