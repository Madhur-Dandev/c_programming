/* This B-Tree algorithm do not permit duplicates
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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
Node *make_node(int, Key *arr[]);
Key *add_adjt(Node *, char *, ...);
void insert_main(Node *, Key **, int);
void insert(Node **, int);
void inorder(Node *);
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
	//printf("%p %p %p %p %p %p %p %p\n", root->arr[0]->left, root->arr[0]->left->arr[0]->left, root->arr[0]->left->arr[0]->right, root->arr[0]->right, root->arr[0]->right->arr[0]->left, root->arr[0]->right->arr[0]->right, root->arr[0]->right->arr[1]->left, root->arr[0]->right->arr[1]->right);
	//printf("%d %d %d\n", root->size, root->arr[0]->left->size, root->arr[0]->right->size);
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

Node *make_node(int count, Key *arr[])
{
	Node *new = (Node *) calloc(1, sizeof(Node));
	if(new == NULL)
	{
		perror("Insertion falied");
		return NULL;
	}

	Key *new_key;
	for(int i = 0; i < count; i++)
	{
		new->arr[i] = arr[i];
	}
	new->size = count;
	return new;
}

Key *add_adjt(Node *node, char *type, ...)
{
	va_list arg;
	va_start(arg, type);
	Key *new = NULL;
	if(type[0] == 'k')
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
	
	// finding place in current array of keys
	int idx = 0;
	for(; idx < node->size; idx++)
	{
		if(node->arr[idx]->value < new->value)
			continue;
		else if(node->arr[idx]->value > new->value)
			break;
		else
			return NULL;
	}

	// determining whether to put in array or break the
	// array in parent-child relationship
	if((node->size + 1) >= DEG)
	{
		// array overflow
		// finding parent and overflowed key
		Key *extra;
		Key *parent;
		int median = (DEG - 1) / 2;
		if(idx == (DEG - 1))
		{
			node->arr[node->size - 1]->right = new->left;
			extra = new;
		}
		else
		{
			// shift digit to right
			Key *temp = node->arr[idx];
			node->arr[idx] = new;
			if(idx - 1 >= 0)
				node->arr[idx - 1]->right = new->left;
			if(idx + 1 < DEG - 1)
				node->arr[idx + 1]->left = new->right;

			add_new(node, new, &extra, idx);
		}

		// takeout parent and insert overflowed node
		parent = node->arr[median];
		for(int i = median; i < node->size - 1; i++)
		{
			node->arr[i] = node->arr[i+1];
		}
		node->arr[node->size - 1] = extra;
	
		Node *new_child = make_node(median, node->arr);
		parent->left = new_child;
		parent->right = node;
		node->size = DEG - median - 1;
		for(int i = 0; i < DEG - 1; i++)
		{
			if(median < DEG - i)
			{
				node->arr[i] = node->arr[median];
				median++;
			}
			else
			{
				node->arr[i] = NULL;
			}
		}
		return parent;
	}
	else
	{
		add_new(node, new, NULL, idx);
		return NULL;
	}
}

void insert_main(Node *root, Key **ret_val,  int value)
{
	int idx = 0;
	for(; idx < root->size; idx++)
		if(root->arr[idx]->value > value)
			break;

	Key *key = NULL;
	if((idx == DEG - 1 || idx == root->size) && root->arr[idx - 1]->right != NULL)
		insert_main(root->arr[idx - 1]->right, &key, value);
	else if(idx < DEG - 1 && idx < root->size  && root->arr[idx]->left != NULL)
		insert_main(root->arr[idx]->left, &key, value);
	else
	{
		*ret_val = add_adjt(root, "i", value);
		return;
	}

	if(key != NULL)
	{
		key = add_adjt(root, "k", key);
		if(key != NULL)
			*ret_val = key;
	}
	return;
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
		insert_main(*root, &key, value);
	}

	if(key != NULL)
	{
		Node *new = make_node(1, (Key *[1]) {key});
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
