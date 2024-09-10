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
	insert(&root, 10);

	printf("Max: %d\n", max(root));
	printf("Min: %d\n", min(root));
	printf("Max: %d\n", max_no_recur(root));
	printf("Min: %d\n", min_no_recur(root));
	printf("Search pointer: %p\tvs\tSearch with no recursion pointer: %p\n", search(root, 2), search_no_recur(root, 2));
	printf("Size of tree: %d\tvs\tSize of tree with no recursion: %d\n", size(root), size_no_recur(root));
	printf("Height of tree: %d\n", get_height(root, 0));
	leaf_nodes_no_recur(root);
	full_nodes_no_recur(root);
	half_nodes_no_recur(root);
	return 0;
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

int max_no_recur(node *n) {
	lln *queue = (lln *) malloc(sizeof(lln)), *tail = queue;
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
			tail->next = node;
			tail = node;

			if(queue->data_node->right) {
				node2 = (lln *) malloc(sizeof(lln));
				if(!node2)
					goto err;

				node2->data_node = queue->data_node->right;
				node2->next = NULL;
				tail->next = node2;
				tail = node2;
			}
		}

		lln *temp = queue;
		queue = queue->next;
		free(temp);
	}

	return max;
}

int min_no_recur(node *n) {
	lln *queue = (lln *) malloc(sizeof(lln)), *tail = queue;
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
			tail->next = node;
			tail = node;

			if(queue->data_node->right) {
				node2 = (lln *) malloc(sizeof(lln));
				if(!node2)
					goto err;

				node2->data_node = queue->data_node->right;
				node2->next = NULL;
				tail->next = node2;
				tail = node2;
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

int get_height(node *n, int lev) {
	if(!n)
		return lev-1;
	return get_height(n->left, lev+1);
}

node *search_no_recur(node *root, int val) {
	node *target = NULL;

	lln *queue, *tail, *temp;
	queue = tail = (lln *) malloc(sizeof(lln));
	if(!queue) {
		err:
			perror("Unable to search for the element.");
			return NULL;
	}

	queue->data_node = root;
	queue->next = NULL;

	while(queue) {
		temp = queue;
		lln *node;
		node = NULL;

		if(temp->data_node->data == val) {
			target = temp->data_node;
			break;
		}

		if(temp->data_node->left) {
			node = (lln *) malloc(sizeof(lln));
			if(!node)
				goto err;

			node->data_node = temp->data_node->left;
			node->next = NULL;
			tail->next = node;
			tail = node;

			if(temp->data_node->right) {
				node = (lln *) malloc(sizeof(lln));
				if(!node)
					goto err;

				node->data_node = temp->data_node->right;
				node->next = NULL;
				tail->next = node;
				tail = node;
			}
		}

		queue = queue->next;
		free(temp);
	}

	lln *queue_free;
	while(queue) {
		queue_free = queue;
		queue = queue->next;
		free(queue_free);
	}

	return target;
}

int size_no_recur(node *root) {
	int counter = 0;
	lln *queue, *tail, *temp;

	queue = tail = (lln *) malloc(sizeof(lln));
	if(!queue) {
		err:
			perror("Unable to search for the element.");
			return 0;
	}

	queue->data_node = root;
	queue->next = NULL;

	while(queue) {
		temp = queue;
		lln *node;
		node = NULL;

		if(temp->data_node->left) {
			node = (lln *) malloc(sizeof(lln));
			if(!node)
				goto err;

			node->data_node = temp->data_node->left;
			node->next = NULL;
			tail->next = node;
			tail = node;

			if(temp->data_node->right) {
				node = (lln *) malloc(sizeof(lln));
				if(!node)
					goto err;

				node->data_node = temp->data_node->right;
				node->next = NULL;
				tail->next = node;
				tail = node;
			}
		}

		queue = queue->next;
		free(temp);
		counter++;
	}

//	lln *queue_free;
//	while(queue) {
//		queue_free = queue;
//		queue = queue->next;
//		free(queue_free);
//	}

	return counter;
}

int leaf_nodes_no_recur(node *root) {
	printf("Leaf nodes: ");

	lln *queue, *tail, *temp;

	queue = tail = (lln *) malloc(sizeof(lln));
	if(!queue) {
		err:
			perror("Unable to find the leaf nodes.");
			return 0;
	}

	queue->data_node = root;
	queue->next = NULL;

	while(queue) {
		temp = queue;
		lln *node;
		node = NULL;

		if(temp->data_node->left == NULL &&
		   temp->data_node->right == NULL) {
			printf("%p: %d\n", temp->data_node, temp->data_node->data);
		}

		if(temp->data_node->left) {
			node = (lln *) malloc(sizeof(lln));
			if(!node)
				goto err;

			node->data_node = temp->data_node->left;
			node->next = NULL;
			tail->next = node;
			tail = node;

			if(temp->data_node->right) {
				node = (lln *) malloc(sizeof(lln));
				if(!node)
					goto err;

				node->data_node = temp->data_node->right;
				node->next = NULL;
				tail->next = node;
				tail = node;
			}
		}

		queue = queue->next;
		free(temp);
	}
	return 0;
}

int full_nodes_no_recur(node *root) {
	printf("Full nodes: ");
	lln *queue, *tail, *temp;

	queue = tail = (lln *) malloc(sizeof(lln));
	if(!queue) {
		err:
			perror("Unable to find the full nodes.");
			return 0;
	}

	queue->data_node = root;
	queue->next = NULL;

	while(queue) {
		temp = queue;
		lln *node;
		node = NULL;

		if(temp->data_node->left != NULL &&
		   temp->data_node->right != NULL) {
			printf("%p: %d\n", temp->data_node, temp->data_node->data);
		}

		if(temp->data_node->left) {
			node = (lln *) malloc(sizeof(lln));
			if(!node)
				goto err;

			node->data_node = temp->data_node->left;
			node->next = NULL;
			tail->next = node;
			tail = node;

			if(temp->data_node->right) {
				node = (lln *) malloc(sizeof(lln));
				if(!node)
					goto err;

				node->data_node = temp->data_node->right;
				node->next = NULL;
				tail->next = node;
				tail = node;
			}
		}

		queue = queue->next;
		free(temp);
	}
	return 0;
}

int half_nodes_no_recur(node *root) {
	printf("Half nodes: ");
	lln *queue, *tail, *temp;

	queue = tail = (lln *) malloc(sizeof(lln));
	if(!queue) {
		err:
			perror("Unable to find the half nodes.");
			return 0;
	}

	queue->data_node = root;
	queue->next = NULL;

	while(queue) {
		temp = queue;
		lln *node;
		node = NULL;

		if(temp->data_node->left != NULL &&
		   temp->data_node->right == NULL) {
			printf("%p: %d\n", temp->data_node, temp->data_node->data);
		}

		if(temp->data_node->left) {
			node = (lln *) malloc(sizeof(lln));
			if(!node)
				goto err;

			node->data_node = temp->data_node->left;
			node->next = NULL;
			tail->next = node;
			tail = node;

			if(temp->data_node->right) {
				node = (lln *) malloc(sizeof(lln));
				if(!node)
					goto err;

				node->data_node = temp->data_node->right;
				node->next = NULL;
				tail->next = node;
				tail = node;
			}
		}

		queue = queue->next;
		free(temp);
	}
	return 0;
}
