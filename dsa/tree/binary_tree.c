/* This file contains some basic functions
 * of binary tree like (insertion, deletion,
 * get size, get height)
 */

#include <stdio.h>
#include "btree.h"

/*int main(void) {
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
	insert(&root, 5);
	insert(&root, 6);
	insert(&root, 10);
	insert(&root, 1);
	insert(&root, 2);
	insert(&root, 200);
	insert(&root, -19);
	preorder(root);
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
	printf("%d\n", get_height(root, 0));
	return 0;
} */

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
		while(queue != NULL) {
			temp = queue;
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
					temp1->data_node = temp->data_node->right;
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

int delete(node **root) {
	struct del_ds *queue, *tail, *temp;
	node *to_free;
	queue = tail = (struct del_ds *) malloc(sizeof(struct del_ds));
	queue->data_node = *root;
	queue->next = NULL;
	queue->parent = NULL;

	while(queue) {
		temp = queue;
		if(temp->data_node->left == NULL &&
		   temp->data_node->right == NULL)
			break;

		if(temp->data_node->left != NULL) {
			struct del_ds *temp1 = (struct del_ds *)malloc(sizeof(struct del_ds));
			temp1->data_node = temp->data_node->left;
			temp1->parent = temp->data_node;
			temp1->next = NULL;
			tail->next = temp1;
			tail = tail->next;
			if(tail->data_node->right != NULL) {
				temp1 = (struct del_ds *) malloc(sizeof(struct del_ds));
				temp1->data_node = temp->data_node->right;
				temp1->parent = temp->data_node;
				temp1->next = NULL;
				tail->next = temp1;
				tail = tail->next;
			}
		}

		queue = queue->next;
		free(temp);
	}

	if(tail->parent == NULL) {
		to_free = *root;
		free(to_free);
		*root = NULL;
	}
	else {
		node *parent = tail->parent;
		if(parent->right != NULL) {
			to_free = parent->right;
			free(to_free);
			parent->right = NULL;
		}
		else {
			to_free = parent->left;
			free(to_free);
			parent->left = NULL;
		}
	}

	struct del_ds *queue_free;
	while(queue) {
		queue_free = queue;
		queue = queue->next;
		free(queue_free);
	}

	return 0;
}
