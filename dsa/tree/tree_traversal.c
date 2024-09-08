#include <stdio.h>
#include "btree.h"

int main(void) {
	node *root = NULL;
	insert(&root, 1);
	insert(&root, 2);
	insert(&root, 3);
	insert(&root, 4);
	insert(&root, 5);
	insert(&root, 6);
	insert(&root, 7);
	insert(&root, 8);
	insert(&root, 9);
	delete(&root);
	printf("Preorder: ");
	preorder(root);
	puts("");
	printf("Inorder: ");
	inorder(root);
	puts("");
	printf("Postorder: ");
	postorder(root);
	preorder_no_recur(root);
	postorder_no_recur(root);
	printf("Level order: ");
	levelorder(root);
	reverse_levelorder(root);
}

void preorder(node *node) {
	if(!node)
		return;
	printf("%d ", node->data);
	preorder(node->left);
	preorder(node->right);
}

void inorder(node *node) {
	if(!node)
		return;
	inorder(node->left);
	printf("%d ", node->data);
	inorder(node->right);
}

void postorder(node *node) {
	if(!node)
		return;
	postorder(node->left);
	postorder(node->right);
	printf("%d ", node->data);
}

void levelorder(node *n) {
	lln *queue = (lln *) malloc(sizeof(lln)), *tracker = queue;
	if(!queue) {
		err:
			perror("Unable to print level order wise");
			return;
	}

	queue->data_node = n;
	queue->next = NULL;

	while(queue) {
		printf("%d ", queue->data_node->data);

		lln *node, *node2;
		node = node2 = NULL;

		if(queue->data_node->left) {
			node = (lln *) malloc(sizeof(lln));
			if(!node)
				goto err;

			node->data_node = queue->data_node->left;
			node->next = NULL;
			tracker->next = node;
			tracker = node;

			if(queue->data_node->right) {
				node2 = (lln *) malloc(sizeof(lln));
				if(!node2)
					goto err;

				node2->data_node = queue->data_node->right;
				node2->next = NULL;
				tracker->next = node2;
				tracker = node2;
			}
		}

		lln *temp = queue;
		queue = queue->next;
		free(temp);

	}
	puts("");
}

void preorder_no_recur(node *n) {
	printf("\nPreorder No recursion: ");
	struct tree_stack *stack = (struct tree_stack *) malloc(sizeof(struct tree_stack));
	lln *queue = NULL, *head = NULL;
	stack->data_node = n;
	stack->prev = stack->next = NULL;

	while(stack) {
		struct tree_stack *node = NULL, *node2 = NULL;
		lln *temp = (lln *) malloc(sizeof(lln));
		temp->data_node = stack->data_node;
		temp->next = NULL;
		if(queue) {
			queue->next = temp;
			queue = queue->next;
		}
		else
			head = queue = temp;

		if(stack->data_node->left) {
			node = (struct tree_stack *) malloc(sizeof(struct tree_stack));
			node->data_node = stack->data_node->left;
			node->next = node->prev = NULL;
			if(stack->data_node->right) {
				node2 = (struct tree_stack *) malloc(sizeof(struct tree_stack));
				node2->data_node = stack->data_node->right;
				node2->next = node;
				node->prev = node2;
			}
		}
		struct tree_stack *temp_node = stack;
		stack = stack->prev;
		if(stack) {
			if(node2) {
				node2->prev = stack;
				stack->next = node2;
			}
			if(!node2 && node) {
				node->prev = stack;
				stack->next = node;
			}
		}
		if(node)
			stack = node;

		free(temp_node);
	}

	while(head) {
		lln *temp_node = head;
		printf("%d ", head->data_node->data);
		head = head->next;
		free(temp_node);
	}
}

void postorder_no_recur(node *n) {
	printf("\nPostorder No recursion: ");
	struct tree_stack *head, *final, *temp;
	head = final = temp = NULL;

	head = (struct tree_stack *) malloc(sizeof(struct tree_stack));

	if(head == NULL)
		return;

	head->data_node = n;
	head->prev = head->next = NULL;

	while(head) {
		temp = head;
		head = head->prev;
		temp->next = temp->prev = NULL;
		if(final == NULL)
			final = temp;
		else {
			final->next = temp;
			temp->prev = final;
			final = final->next;
		}

		struct tree_stack *new = (struct tree_stack *) malloc(sizeof(struct tree_stack));

		if(new == NULL)
			return;

		if(temp->data_node->left) {
			new->data_node = temp->data_node->left;
			new->prev = new->next = NULL;

			if(head == NULL) {
				head = new;
			}
			else {
				head->next = new;
				new->prev = head;
				head = head->next;
			}

			if(temp->data_node->right) {
				new = (struct tree_stack *) malloc(sizeof(struct tree_stack));
				new->data_node = temp->data_node->right;
				new->prev = new->next = NULL;

				head->next = new;
				new->prev = head;
				head = head->next;
			}
		}
		else
			free(new);
	}

	while(final) {
		temp = final;
		printf("%d ", final->data_node->data);
		final = final->prev;
		free(temp);
	}

	puts("");
}

void reverse_levelorder(node *n) {
	printf("Reverse level order: ");
	lln *queue = NULL, *tail = NULL;
	tail = queue = (lln *) malloc(sizeof(lln));
	tail->data_node = n;
	tail->next = NULL;
	struct tree_stack *stack = NULL;

	lln *temp = NULL;

	while(queue) {
		if(!stack) {
			stack = (struct tree_stack *) malloc(sizeof(struct tree_stack));
			stack->data_node = queue->data_node;
			stack->next = stack->prev = NULL;
		}
		else {
			struct tree_stack *temp_stack_node = (struct tree_stack *) malloc(sizeof(struct tree_stack));
			temp_stack_node->data_node = queue->data_node;
			temp_stack_node->next = NULL;
			temp_stack_node->prev = stack;
			stack->next = temp_stack_node;
			stack = temp_stack_node;
		}

		lln *to_free = queue;
		if(to_free->data_node->right) {
			temp = (lln *) malloc(sizeof(lln));
			temp->data_node = to_free->data_node->right;
			temp->next = NULL;
			tail->next = temp;
			tail = temp;
		}

		if(to_free->data_node->left) {
			temp = (lln *) malloc(sizeof(lln));
			temp->data_node = to_free->data_node->left;
			temp->next = NULL;
			tail->next = temp;
			tail = temp;
		}
		queue = queue->next;
		free(to_free);
	}

	while(stack) {
		printf("%d ", stack->data_node->data);
		stack = stack->prev;
	}

	printf("\n");
}
