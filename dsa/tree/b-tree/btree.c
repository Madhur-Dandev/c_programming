/* This B-Tree algorithm do not permit duplicates
 */

// Change the linear search to binary search for faster operation.

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>

#define DEG 4
#define ARR_RGHT_SFT

struct node
{
	int size;
	struct key_node *arr[DEG];
};

struct key_node
{
	int value;
	struct node *left;
	struct node *right;
};

typedef struct node Node;
typedef struct key_node Key;

void add_new(Node *, Key *, Key **, int);
Node *make_node(int, int, Key *arr[]);
Key *add_adjt(Node *, char, int, ...);
// void insert_main(Node *, Key **, int);
void insert(Node **, int);
void inorder(Node *);
void delete(Node **, int);
void delete_tree(Node *);

int main(void)
{
	// make_node(5, 1, 2, 3, 4, 5);
	Node *root = NULL;
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
	insert(&root, 11);
	insert(&root, 12);
	insert(&root, 13);
	insert(&root, 14);
	insert(&root, 15);
	insert(&root, 16);
	insert(&root, 17);
	insert(&root, 18);
	insert(&root, 19);
	delete(&root, 17);
	/*insert(&root, 0);
	insert(&root, -1);
	insert(&root, -2);
	insert(&root, -3);*/
	delete(&root, 1);
	delete(&root, 2);
	delete(&root, 3);
	delete(&root, 4);
	delete(&root, 5);
	delete(&root, 6);
	delete(&root, 7);
	delete(&root, 8);
	delete(&root, 9);
	delete(&root, 10);
	delete(&root, 11);
	delete(&root, 12);
	delete(&root, 13);
	delete(&root, 14);
	delete(&root, 15);
	delete(&root, 16);
	delete(&root, 17);
	delete(&root, 18);
	delete(&root, 19);
	// delete(&root, 100);
	// printf("root %p\n", root);
	// delete(&root, 8);
	// printf("value %d\n", root->arr[0]->left->arr[1]->value);
	inorder(root);
	delete_tree(root);
	return 0;
}

void add_new(Node *node, Key *to_add, Key **ovrflw, int pos)
{
	Key *temp = to_add;
	Key *extra;
	for (int i = pos; i <= node->size; i++)
	{
		if (i == DEG - 1)
			break;
		extra = node->arr[i];
		node->arr[i] = temp;
		temp = extra;
	}

	if (ovrflw != NULL)
	{
		*ovrflw = extra;
		return;
	}

	node->size++;
	return;
}

Node *make_node(int count, int strt, Key *arr[])
{
	Node *new = (Node *)calloc(1, sizeof(Node));
	if (new == NULL)
	{
		perror("Insertion falied");
		return NULL;
	}

	for (int i = 0; i < count; i++)
	{
		new->arr[i] = arr[strt + i];
	}
	new->size = count;
	return new;
}

Key *add_adjt(Node *node, char type, int idx, ...)
{
	va_list arg;
	va_start(arg, idx);
	Key *new = NULL;
	if (type == 'k')
	{
		new = va_arg(arg, Key *);
	}
	else
	{
		new = (Key *)calloc(1, sizeof(Key));
		if (new == NULL)
		{
			perror("Insertion Failed");
			return NULL;
		}
		new->value = va_arg(arg, int);
	}
	va_end(arg);

	Key *parent = NULL;

	if (idx == 0 && new->right != NULL)
		node->arr[idx]->left = new->right;

	Key *extra = new;
	Key *temp;
	for (int i = idx; i < node->size; i++)
	{
		temp = node->arr[i];
		node->arr[i] = extra;
		extra = temp;
	}

	if (node->size + 1 == DEG)
	{
		int median = floor((DEG - 1) / 2);
		parent = node->arr[median];
		for (int i = median; i < node->size - 1; i++)
			node->arr[i] = node->arr[i + 1];
		node->arr[node->size - 1] = extra;

		Node *rgh_ch = make_node(node->size - median, median, node->arr);
		parent->left = node;
		parent->right = rgh_ch;
		node->size = median;
	}
	else
	{
		node->arr[node->size] = extra;
		node->size++;
	}

	return parent;
}

Key *insert_main(Node *root, int value)
{
	int idx = 0;
	for (; idx < root->size; idx++)
		if (root->arr[idx]->value > value)
			break;

	if (idx > 0 && root->arr[idx - 1]->value == value)
		return NULL;

	Key *key = NULL;
	if ((idx == DEG - 1 || idx == root->size) && root->arr[idx - 1]->right != NULL)
		key = insert_main(root->arr[idx - 1]->right, value);
	else if (idx < root->size && root->arr[idx]->left != NULL)
		key = insert_main(root->arr[idx]->left, value);
	else
		return add_adjt(root, 'i', idx, value);

	if (key != NULL)
	{
		key = add_adjt(root, 'k', idx, key);
	}
	return key;
}

void insert(Node **root, int value)
{
	Key *key = NULL;
	if (*root == NULL)
	{
		key = (Key *)calloc(1, sizeof(Key));
		key->value = value;
	}
	else
	{
		key = insert_main(*root, value);
	}

	if (key != NULL)
	{
		Node *new = make_node(1, 0, (Key *[1]){key});
		*root = new;
	}
	return;
}

void inorder(Node *root)
{
	if (root == NULL)
		return;

	for (int i = 0; i < root->size; i++)
	{
		inorder(root->arr[i]->left);
		printf("%d\n", root->arr[i]->value);
	}
	inorder(root->arr[root->size - 1]->right);
}

Key *delete_main(Node *root, Node **ret_val, int value, bool preds_srch)
{
	Key *to_free = NULL;
	Node *lost_link = NULL;
	int idx = -1;

	if (root == NULL)
		return NULL;

	if (preds_srch)
	{
		idx = root->size - 1;
		to_free = delete_main(root->arr[idx]->right, &lost_link, value, preds_srch);
	}
	else
	{
		idx = 0;
		for (; idx < root->size; idx++)
			if (root->arr[idx]->value >= value)
				break;

		if (idx == root->size)
		{
			puts("here");
			idx -= 1;
		}

		// if(idx >= DEG - 1)
		if (root->arr[idx]->value < value)
		{
			to_free = delete_main(root->arr[idx]->right, &lost_link, value, preds_srch); // index is root->size - 1 because, search overflowed the index
		}
		// else
		//{
		//	if(root->arr[idx]->value != value)
		if (root->arr[idx]->value > value)
		{
			to_free = delete_main(root->arr[idx]->left, &lost_link, value, preds_srch);
		}
	}

	if (to_free == NULL)
	{
		// if(idx >= DEG - 1)
		//	idx = idx - 1;
		if (root->arr[idx]->value == value || preds_srch)
			to_free = root->arr[idx];
	}

	printf("key_value: %d, value: %d, index: %d, to_free: %p, key: %p\n", root->arr[idx]->value, value, idx, to_free, root->arr[idx]);
	// return to_free;
	if (to_free == root->arr[idx])
	{
		if (to_free->left == NULL && to_free->right == NULL)
		{
			for (int i = idx; i < root->size - 1; i++)
				root->arr[i] = root->arr[i + 1];

			root->size--;
		}
		else
		{
			Node *lf = root->arr[idx]->left;
			Node *rgh = root->arr[idx]->right;
			Key *rplc_key = delete_main(lf, &lost_link, 0, true);
			printf("rplc_key: %p rplc_key_value: %d\n", rplc_key, rplc_key->value);
			rplc_key->left = lf;
			rplc_key->right = rgh;

			// if(idx > 0)
			//	rplc_key->left = root->arr[idx - 1]->right;
			// if(idx < DEG - 2 && root->size > 1)
			//	rplc_key->right = root->arr[idx + 1]->left;
			root->arr[idx] = rplc_key;
		}
	}

	if (root->arr[idx]->left != NULL && root->arr[idx]->right != NULL)
	{
		int min_key = (int)floor((DEG - 1) / 2);
		bool merge = false;

		if (root->arr[idx]->right->size < min_key)
		{
			if (root->arr[idx]->left->size > min_key)
			{
				Node *lf_chd = root->arr[idx]->left;
				Node *rgh_chd = root->arr[idx]->right;
				Key *lf_node_key = lf_chd->arr[lf_chd->size - 1];
				root->arr[idx]->left = lf_node_key->right;
				root->arr[idx]->right = rgh_chd->size > 0 ? rgh_chd->arr[0]->left : lost_link;

				Key *to_put = root->arr[idx];
				Key *to_hold;
				for (int i = 0; i <= rgh_chd->size; i++)
				{
					to_hold = rgh_chd->arr[i];
					rgh_chd->arr[i] = to_put;
					to_put = to_hold;
				}
				rgh_chd->size++;

				lf_node_key->left = lf_chd;
				lf_node_key->right = rgh_chd;
				root->arr[idx] = lf_node_key;
				lf_chd->size--;
			}
			else
			{
				merge = true;
			}
		}

		if (root->arr[idx]->left->size < min_key)
		{
			if (idx > 0 && root->arr[idx - 1]->left->size > min_key)
			{
				Node *sib_lf_chd = root->arr[idx - 1]->left;
				Node *sib_rgh_chd = root->arr[idx]->left;
				Key *sib_lf_node_key = sib_lf_chd->arr[sib_lf_chd->size - 1];

				Key *sib = root->arr[idx - 1];
				sib->left = sib_lf_node_key->right;
				sib->right = sib_rgh_chd->size > 0 ? sib_rgh_chd->arr[0]->left : lost_link;

				Key *to_put = root->arr[idx];
				Key *to_hold;
				for (int i = 0; i <= sib_rgh_chd->size; i++)
				{
					to_hold = sib_rgh_chd->arr[i];
					sib_rgh_chd->arr[i] = to_put;
					to_put = to_hold;
				}
				sib_rgh_chd->size++;

				sib_lf_node_key->left = sib_lf_chd;
				sib_lf_node_key->right = sib_rgh_chd;
				root->arr[idx - 1] = sib_lf_node_key;
				sib_lf_chd->size--;
			}
			else if (root->arr[idx]->right->size > min_key)
			{
				Node *lf_chd = root->arr[idx]->left;
				Node *rgh_chd = root->arr[idx]->right;
				Key *lf_node_key = rgh_chd->arr[0];

				root->arr[idx]->left = lf_chd->size > 0 ? lf_chd->arr[lf_chd->size - 1]->right : lost_link;
				root->arr[idx]->right = rgh_chd->arr[0]->left;

				lf_chd->arr[lf_chd->size] = root->arr[idx];
				lf_chd->size++;

				lf_node_key->left = lf_chd;
				lf_node_key->right = rgh_chd;
				root->arr[idx] = lf_node_key;

				for (int i = 0; i < rgh_chd->size - 1; i++)
					rgh_chd->arr[i] = rgh_chd->arr[i + 1];
				rgh_chd->size--;
			}
			else
			{
				merge = true;
			}
		}

		if (merge)
		{
			*ret_val = root->arr[idx]->left;

			Node *rgh_ch = root->arr[idx]->right;
			root->arr[idx]->left = (*ret_val)->size > 0 ? (*ret_val)->arr[(*ret_val)->size - 1]->right : lost_link;
			root->arr[idx]->right = rgh_ch->size > 0 ? rgh_ch->arr[0]->left : NULL;

			(*ret_val)->arr[(*ret_val)->size] = root->arr[idx];
			(*ret_val)->size++;

			for (int i = 0; i < rgh_ch->size; i++)
			{
				(*ret_val)->arr[(*ret_val)->size + i] = rgh_ch->arr[i];
			}

			(*ret_val)->size += rgh_ch->size;
			free(rgh_ch);

			if (root->size > 1)
			{
				if (idx > 0)
					root->arr[idx - 1]->right = *ret_val;
				if (idx < DEG - 2)
					root->arr[idx + 1]->left = *ret_val;
				for (int i = idx; i < root->size - 1; i++)
					root->arr[i] = root->arr[i + 1];
				*ret_val = NULL;
			}
			root->size--;
		}
	}

	return to_free;
}

void delete(Node **root, int value)
{
	Node *lost_link = NULL;
	Key *to_free = delete_main(*root, &lost_link, value, false);
	if (to_free == NULL)
		return;

	printf("value %d, root_size: %d, root: %p, lost_link: %p, to_free: %p\n", value, (*root)->size, *root, lost_link, to_free);
	if ((*root)->size == 0 || lost_link)
	{
		free(*root);
		*root = lost_link;
	}

	printf("delete: %p\n", to_free);
	free(to_free);
	return;
}

void delete_tree(Node *root)
{
	if (root == NULL)
		return;

	for (int i = 0; i < root->size; i++)
	{
		delete_tree(root->arr[i]->left);

		if (i == root->size - 1)
			delete_tree(root->arr[i]->right);

		free(root->arr[i]);
	}
}
