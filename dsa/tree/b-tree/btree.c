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

int main(void)
{
	//make_node(5, 1, 2, 3, 4, 5);
	Node *root = NULL;
	insert(&root, 1);
	insert(&root, 2);
	insert(&root, 3);
	insert(&root, 4);
	/*insert(&root, 5);
	insert(&root, 6);*/

	return 0;
}

void add_new(Node *node, Key *to_add, Key **ovrflw, int pos)
{
	Key *temp = to_add;
	Key *extra;
	for(int i = pos; i < node->size; i++)
	{
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

	//printf("%d\n", new->size);
	Key *new_key;
	for(int i = 0; i < count; i++)
	{
		new->arr[i] = arr[i];
		/*new_key = (Key *) calloc(1, sizeof(Key));
		if(new_key == NULL)
		{
			perror("Insertion falied");
			for(int j = 0; j < i; i++)
			{
				free(new->arr[j]);
			}
			free(new);
			return NULL;
		}
		new_key->value = arr[i];
		new->arr[i] = new_key;*/
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

			/*for(int i = idx; i < node->size; i++)
			{
				extra = node->arr[i];
				node->arr[i] = temp;
				//node->arr[i + 1]->left = new->arr[i]->right;
				temp = extra;
			}*/
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
		// fix this
		/*Key *temp = node->arr[idx];
		Key *next;
		node->arr[idx] = new;
		if(idx - 1 != 0)
			node->arr[idx - 1]->right = new->left;
		if(idx + 1 != DEG - 1)
			node->arr[idx + 1]->left = new->right;

		for(int i = idx; i < node->size - 1; i++)
		{
			next = node->arr[i + 1];
			node->arr[i + 1] = temp;
			node->arr[i + 1]->left = node->arr[i]->right;
			temp = extra;
		}*/
		add_new(node, new, NULL, idx);
		return NULL;
	}
}

void insert_main(Node *root, Key **ret_val,  int value)
{
	/*if(root == NULL)
	{
		//Node *new = make_node(1, value);
		return;
	}*/

	int idx = 0;
	while(idx < root->size)
	{	
		if(value > root->arr[idx]->value)
		{
			if(idx == root->size - 1)
				break;
			else
				idx++;
			continue;
		}
		break;
	}

	Key *key = NULL;
	if(root->arr[idx]->value > value && root->arr[idx]->left != NULL)
		insert_main(root->arr[idx]->left, &key, value);
	else if(root->arr[idx]->value < value && root->arr[idx]->right != NULL)
		insert_main(root->arr[idx]->right, &key, value);
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

	/*Key *new = (Key *) calloc(1, sizeof(Key));
	if(new == NULL)
	{
		perror("Insertion Failed");
		return;
	}
	new->value = value;

	// finding place in current array of keys
	int idx = 0;
	for(; idx < root->size; idx++)
	{
		if(root->arr[idx]->value < value)
			continue;
		else if(root->arr[idx]->value > value)
			break;
		else
			return;
	}

	Node *child_ret_val = NULL;
	if(value > root->arr[size-1])
		return;

	// determining whether to put in array or break the
	// array in parent-child relationship
	if((root->size + 1) >= DEG)
	{
		// array overflow
		// finding parent and overflowed key
		Key *extra;
		Key *parent;
		int median = (DEG - 1) / 2;
		if(idx >= (DEG - 1))
		{f
			extra = new;
		}
		else
		{
			// shift digit to right
			Key *temp = root->arr[idx];
			root->arr[idx] = new;
			for(int i = idx; i < root->size - 1; i++)
			{
				extra = root->arr[i + 1];
				root->arr[i + 1] = temp;
				temp = extra;
			}
		}	
		parent = root->arr[median];
		for(int i = median; i < size; i++)
		{
			
		}
		root->arr[size-1] = extra;
	}
	else
	{}
	return;*/
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
	if(root->size == 0)
		return;

	for(int i = 0; i < root->size; i++)
	{
		inorder(root->arr[i]->left);
		printf("%d\n", root->arr[i]->value);

		if(i + 1 >= root->size)
			inorder(root->arr[i]->right);
	}
}
