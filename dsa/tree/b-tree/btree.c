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
Key *add_adjt(Node *, char , int, ...);
//void insert_main(Node *, Key **, int);
void insert(Node **, int);
void inorder(Node *);
void delete(Node *, int);
void delete_tree(Node *);

int main(void)
{
	//make_node(5, 1, 2, 3, 4, 5);
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
	printf("value %d\n", root->arr[0]->right->arr[0]->value);
	inorder(root);
	delete_tree(root);
	return 0;
}

void add_new(Node *node, Key *to_add, Key **ovrflw, int pos)
{
	Key *temp = to_add;
	Key *extra;
	for(int i = pos; i <= node->size; i++)
	{
		if(i == DEG - 1)
			break;
		extra = node->arr[i];
		node->arr[i] = temp;
		temp = extra;
	}
	
	if(ovrflw != NULL)
	{
		*ovrflw = extra;
		return;
	}

	node->size++;
	return;
}

Node *make_node(int count, int strt, Key *arr[])
{
	Node *new = (Node *) calloc(1, sizeof(Node));
	if(new == NULL)
	{
		perror("Insertion falied");
		return NULL;
	}

	for(int i = 0; i < count; i++)
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
	if(type == 'k')
	{
		new = va_arg(arg, Key *);
	}
	else
	{	
		new = (Key *) calloc(1, sizeof(Key));
		if(new == NULL)
		{
			perror("Insertion Failed");
			return NULL;
		}
		new->value = va_arg(arg, int);
	}
	va_end(arg);	
		
	Key *parent = NULL;
	
	if(idx == 0 && new->right != NULL)
		node->arr[idx]->left = new->right;

	Key *extra = new;
	Key *temp;
	for(int i = idx; i < node->size; i++)
	{
		temp = node->arr[i];
		node->arr[i] = extra;
		extra = temp;
	}

	if(node->size + 1 == DEG)
	{
		int median = floor((DEG - 1) / 2);
		parent = node->arr[median];
		for(int i = median; i < node->size - 1; i++)
			node->arr[i] = node->arr[i + 1];
		node->arr[node->size - 1] = extra;

		if(parent->value == 4)
			for(int i = 0; i < node->size; i++)
				printf("%d pine %d\n", new->value, node->arr[i]->value); 

		Node *rgh_ch = make_node(node->size - median, median, node->arr);
		parent->left = node;
		parent->right = rgh_ch;
		node->size = median;
	}
	else
	{
		node->arr[node->size] = new;
		node->size++;
	}

	return parent;
}

Key *insert_main(Node *root,  int value)
{
	int idx = 0;
	for(; idx < root->size; idx++)
		if(root->arr[idx]->value > value)
			break;

	if(root->arr[idx - 1]->value == value)
		return NULL;

	Key *key = NULL;
	if((idx == DEG - 1 || idx == root->size) && root->arr[idx - 1]->right != NULL)
		key = insert_main(root->arr[idx - 1]->right, value);
	else if(idx < root->size && root->arr[idx]->left != NULL)
		key = insert_main(root->arr[idx]->left, value);
	else
	{
		return add_adjt(root, 'i', idx, value);
	}

	if(key != NULL)
	{
		key = add_adjt(root, 'k', idx, key);
	}
	return key;
}

void insert(Node **root, int value)
{
	Key *key = NULL;
	if(*root == NULL)
	{
		key = (Key *) calloc(1, sizeof(Key));
		key->value = value;
	}
	else
	{
		key = insert_main(*root, value);
	}

	if(key != NULL)
	{
		Node *new = make_node(1, 0, (Key *[1]) {key});
		*root = new;
	}
	return;
}

void inorder(Node *root)
{
	if(root == NULL)
		return;

	for(int i = 0; i < root->size; i++)
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

	if(root == NULL)
		return NULL;
	
	if(preds_srch)
	{
		idx = root->size - 1;
		to_free = delete_main(root->arr[idx]->right, &lost_link, 0, preds_srch);
		if(to_free == NULL)
		{
			to_free = root->arr[root->size - 1];
			root->size--;
		}
	}
	else
	{
		idx = 0;
		for(; idx < root->size; idx++)
			if(root->arr[idx]->value >= value)
				break;

		
		if(idx >= DEG - 1)
		{
			to_free = delete_main(root->arr[root->size - 1]->right, &lost_link, value, preds_srch); // index is root->size - 1 because, search overflowed the index
		}
		else
		{
			if(root->arr[idx]->value != value)
			{
				to_free = delete_main(root->arr[idx]->left, &lost_link, value, preds_srch);
			}
			else
			{
				to_free = root->arr[idx];
				if(root->arr[idx]->left != NULL)
				{
					Key *sub = delete_main(root->arr[idx]->left, &lost_link, 0, true);// substitution
					sub->left = root->arr[idx]->left;
					sub->right = root->arr[idx]->right;
					root->arr[idx] = sub;
				}
				else
				{
					for(int i = idx; i < root->size - 1; i++)
						root->arr[i] = root->arr[i + 1];
				}
				root->size--;
			}
		}
	}

	if(to_free != NULL && idx > root->size)
	{
		int min_key = (int) ceil((int) DEG / 2.00) - 1;
		bool merge = false;
		Key *rep_key = NULL;
		if(root->arr[idx]->left->size < min_key)
		{
			if(idx > 0 && root->arr[idx - 1]->left->size > min_key)
			{
				Node *sib_chd = root->arr[idx - 1]->left;
				rep_key = root->arr[idx - 1];
				rep_key->left = sib_chd->arr[sib_chd->size - 1]->right;
				sib_chd->arr[sib_chd->size - 1]->left = sib_chd;
				root->arr[idx - 1] = sib_chd->arr[sib_chd->size - 1];
				root->arr[idx - 1]->right = root->arr[idx]->right;
				sib_chd->size--;
				rep_key->right = root->arr[idx]->left->size != 0 ? root->arr[idx]->left->arr[0]->left : lost_link;

				Key *temp;
				for(int i = 0; i <= root->arr[idx]->left->size; i++)
				{
					temp = root->arr[idx]->left->arr[i];
					root->arr[idx]->left->arr[i] = rep_key;
					rep_key = temp;
				}
				root->arr[idx]->left->size++;
			}
			else if(root->arr[idx]->right->size > min_key)
			{
				Node *sib_chd = root->arr[idx]->right;
				Node *lf_ch = root->arr[idx]->left;
				rep_key = root->arr[idx];
				rep_key->right = sib_chd->arr[0]->left;
				sib_chd->arr[0]->left = lf_ch;
				sib_chd->arr[0]->right = sib_chd;
				root->arr[idx] = sib_chd->arr[0];
				for(int i = 0; i < sib_chd->size - 1; i++)
					sib_chd->arr[i] = sib_chd->arr[i + 1];
				sib_chd->size--;
				rep_key->left = root->arr[idx]->left->size != 0 ? root->arr[idx]->left->arr[root->arr[idx]->left->size]->right : lost_link;
				root->arr[idx]->left->arr[root->arr[idx]->left->size] = rep_key;
				root->arr[idx]->left->size++;
			}
			else
			{
				merge = true;
			}
		}
		
		if(root->arr[idx]->right->size < min_key)
		{	
			if(root->arr[idx]->left->size > min_key)
			{
				Node *sib_chd = root->arr[idx]->left;
				Node *lf_ch = root->arr[idx]->right;
				rep_key = root->arr[idx];
				rep_key->left = sib_chd->arr[sib_chd->size - 1]->right;
				sib_chd->arr[sib_chd->size - 1]->left = sib_chd;
				root->arr[idx] = sib_chd->arr[sib_chd->size - 1];
				root->arr[idx]->right = lf_ch;
				sib_chd->size--;
				rep_key->right = root->arr[idx]->right->size != 0 ? root->arr[idx]->right->arr[0]->left : lost_link;

				Key *temp;
				for(int i = 0; i <= root->arr[idx]->left->size; i++)
				{
					temp = root->arr[idx]->left->arr[i];
					root->arr[idx]->left->arr[i] = rep_key;
					rep_key = temp;
				}
			}
			else
			{
				merge = true;
			}
		}

		if(merge)
		{
			Node *lft = root->arr[idx]->left;
			Node *rgh = root->arr[idx]->right;
			root->arr[idx]->left = lft->size > 0 ? lft->arr[lft->size - 1]->right : lost_link;
			lft->arr[lft->size] = root->arr[idx];
			lft->size++;
			lft->arr[lft->size - 1]->right = rgh->size > 0 ? rgh->arr[0]->left : lost_link;

			for(int i = 0; i < rgh->size; i++)
			{
				lft->arr[lft->size + i] = rgh->arr[i];
			}

			lft->size += rgh->size;
			*ret_val = lft;

			if(idx - 1 >= 0)
				root->arr[idx - 1]->right = root->arr[idx]->left;
			if(idx + 1 < DEG - 1)
				root->arr[idx + 1]->left = root->arr[idx]->left;

			for(int i = idx; i < root->size - 1; i++)
				root->arr[i] = root->arr[i + 1];
			root->size--;
			free(rgh);
		}
	}
	
	return to_free;
	// balance algorithm
}

void delete(Node *root, int value)
{
	
	return;
}

void delete_tree(Node *root)
{	
	if(root == NULL)
		return;

	for(int i = 0; i < root->size; i++)
	{
		delete_tree(root->arr[i]->left);
		
		if(i == root->size - 1)
			delete_tree(root->arr[i]->right);

		free(root->arr[i]);
	}
}
