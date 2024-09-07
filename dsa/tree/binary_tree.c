#include <stdio.h>
#include "btree.h"

int main(void) {
	node *root = NULL;
	insert(&root, 1);
	insert(&root, 2);
	insert(&root, 3);
	printf("%d\n", root->left->data);
	insert(&root, 4);
	insert(&root, 5);
	insert(&root, 6);
	insert(&root, 7);
	insert(&root, 8);
	insert(&root, 9);
	insert(&root, 10);
	insert(&root, 11);
	insert(&root, 12);
	insert(&root, 13);
	insert(&root, 14);
	insert(&root, 15);
	insert(&root, 16);
	insert(&root, 17);
	insert(&root, 18);
	insert(&root, 19);
	insert(&root, 20);
	levelorder(root);
	/*insert(&root, 5);
	insert(&root, 6);
	insert(&root, 10);
	insert(&root, 1);
	insert(&root, 2);
	insert(&root, 200);
	insert(&root, -19);*/
	/*preorder(root);
	puts("");
	inorder(root);
	puts("\nPost order");
	postorder(root);
	puts("");
	preorder_no_recurr(root);
	postorder_no_recurr(root);
	puts("");
	levelorder(root);
	puts("");
	int res = max(root);
	printf("%d\n", res);
	res = min(root);
	printf("%d\n", res);
	res = max_no_recc(root);
	printf("%d\n", res);
	res = min_no_recc(root);
	printf("%d\n", res);
	node *result = search(root, 2);
	printf("%p\n", result);
	printf("%d\n", size(root));
	reverse_levelorder(root);
	puts("");
	printf("%d\n", get_height(root, 0));*/
	return 0;
}

int insert(node **root, int val) {
	lln *temp = NULL;
	lln *queue, *tail;
	node *parent = NULL;

	node *new_n = (node *) malloc(sizeof(node));
	new_n->data = val;
	new_n->left = new_n->right = NULL;
	if(*root == NULL) {
		*root = new_n;
	}
	else {
		tail = queue = (lln *) malloc(sizeof(lln));
		queue->data_node = *root;
		queue->next = NULL;
		while(queue) {
			temp = queue;
//			printf("%p %p\n", temp->data_node->left, temp->data_node->right);
			if(temp->data_node->left == NULL ||
			   temp->data_node->right == NULL)
				break;

			if(temp->data_node->left != NULL) {
				lln *temp1 = (lln *)malloc(sizeof(lln));
				temp1->data_node = temp->data_node->left;
				temp1->next = NULL;
				tail->next = temp1;
				tail = tail->next;
				if(tail->data_node->right != NULL) {
					temp1 = (lln *) malloc(sizeof(lln));
					temp1->data_node = tail->data_node->right;
					temp1->next = NULL;
					tail->next = temp1;
					tail = tail->next;
				}
			}
			queue = queue->next;
			free(temp);
		}

		parent = temp->data_node;

		while(queue) {
			temp = queue;
			queue = queue->next;
			free(temp);
		}

	    if(parent->left == NULL)
		    parent->left = new_n;
		else
			parent->right = new_n;
	}

	return 1;
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

void preorder_no_recurr(node *n) {
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

void postorder_no_recurr(node *n) {
	printf("\nPreorder No recursion:");
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
}

int max(node *n) {
    if(!n)
        return INT_MIN;
	int a = max(n->left);
	int b = max(n->right);
	return a > b ?
				a > n->data ?
							a :
							n->data :
				b > n->data ?
							b :
							n->data;
}

int min(node *n) {
    if(!n)
        return INT_MAX;
	int a = min(n->left);
	int b = min(n->right);
	return a < b ?
				a < n->data ?
							a :
							n->data :
				b < n->data ?
							b :
							n->data;
}

int max_no_recc(node *n) {
	lln *queue = (lln *) malloc(sizeof(lln)), *tracker = queue;
	if(!queue) {
		err:
			perror("Unable to print level order wise");
			return INT_MAX;
	}

	queue->data_node = n;
	queue->next = NULL;

	int max;
	if(queue->data_node)
		max = queue->data_node->data;
	else
		return INT_MAX;

	while(queue) {
		if(queue->data_node->data > max)
			max = queue->data_node->data;

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

	return max;
}

int min_no_recc(node *n) {
	lln *queue = (lln *) malloc(sizeof(lln)), *tracker = queue;
	if(!queue) {
		err:
			perror("Unable to print level order wise");
			return INT_MIN;
	}

	queue->data_node = n;
	queue->next = NULL;

	int min;
	if(queue->data_node)
		min = queue->data_node->data;
	else
		return INT_MIN;

	while(queue) {
		if(queue->data_node->data < min)
			min = queue->data_node->data;

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

	return min;
}

node *search(node *n, int val) {
	if(!n)
		return NULL;

	if(n->data == val)
		return n;

	node * res = NULL;
	res = search(n->left, val);
	if(!res)
		res = search(n->right, val);

	return res;
}

int size(node *n) {
	if(!n)
		return 0;
	return (size(n->left) + 1 + size(n->right));
}

void reverse_levelorder(node *n) {
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
}

int get_height(node *n, int lev) {
	if(!n)
		return lev-1;
	return get_height(n->left, lev+1);
}
