#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#ifndef DEG
#define DEG 3
#endif

#define PRNT_MSG puts("Key inserted.");

typedef struct node Node;

struct node
{
	int value_size;
	int children_size;
	int values[DEG - 1];
	Node *children[DEG];
};

#define MAKE_NEW(var) \
do { \
var = (Node *) malloc(sizeof(Node)); \
if(var == NULL) \
	perror("Unable to add new key"); \
\
} while(0);

void insert(Node **, int);
int check_balance(Node *, Node *);
void preorder(Node *);

int main(void)
{
	Node *root = NULL;
	bool pass = false;
	Node *new_chd = NULL;
	/*insert(&root, 1);
	insert(&root, 2);
	insert(&root, 3);
	insert(&root, 4);
	insert(&root, 5);*/

	insert(&root, 12);
	insert(&root, 120);
	insert(&root, 1);
	insert(&root, 51);
	insert(&root, 99);
	insert(&root, 6);
	insert(&root, 1000);
	insert(&root, 721);
	insert(&root, 86);
	insert(&root, 911);
	insert(&root, 20010);
	//insert(&root, 0);
	preorder(root);
	return 0;
}

int insert_main(Node **root, bool *pass, Node **new_chd, int value)
{
	if(*root == NULL)
	{
		Node *new;
		MAKE_NEW(new);
		new->values[new->value_size++] = value;
		*root = new;
		return 0;
	}

	int i = 0, j = (*root)->value_size - 1;
	//printf("%d %d %d %d\n", i, j, value, (*root)->values[0]);
	while(i <= j)
	{
		//puts("abc");
		int med = (i + j) / 2;
		//printf("i: %d, j: %d, med: %d\n", i, j, med);
		if((*root)->values[med] > value)
		{
			//puts("1");
			j = med - 1;
		}
		else if((*root)->values[med] < value)
		{
			//puts("2");
			i = med + 1;
		}
		else
			break;
	}

	bool adjust = true;
	Node *ret_chd = NULL;

	//if(j < (*root)->children_size)
	if((*root)->children_size > 0)
	{
		if(j < 0)
			j = 0;

		if((*root)->values[j] > value)
			i = j - 1 >= 0 ? j - 1 : j;
		else
			i = j + 1 < DEG ? j + 1 : j;

		value = insert_main(&((*root)->children[i]), &adjust, &ret_chd, value);
		//adjust = any_pass;
	}

	if(adjust)
	{
		// using i value to get the insertion point
		int extra = value, temp;
		for(int x = i; x < (*root)->value_size; x++)
		{
			temp = (*root)->values[x];
			(*root)->values[x] = extra;
			extra = temp;
		}

		Node *extra_chd = ret_chd, *temp_chd;

		if(ret_chd != NULL)
		{
			for(int x = i + 1; x < (*root)->children_size; x++)
			{
				temp_chd = (*root)->children[x];
				(*root)->children[x] = extra_chd;
				extra_chd = temp_chd;
			}
		}

		if((*root)->value_size < DEG - 1)
		{
			//(*root)->value_size++;
			(*root)->values[(*root)->value_size++] = extra;
			//(*root)->children_size++;
			if(ret_chd != NULL)
				(*root)->children[(*root)->children_size++] = extra_chd;
			*pass = false;
			return 0;
		}
		else
		{
			*pass = true;
			int med = ceil((double) (DEG - 2) / (double) 2);
			int to_ret = (*root)->values[med];
			Node *new;
			MAKE_NEW(new);
			for(int x = med + 1; x < DEG - 1; x++)
			{
				new->values[new->value_size++] = (*root)->values[x];
				//++(new->value_size);
			}

			new->values[new->value_size++] = extra;
			//++(new->value_size);

			(*root)->value_size = med;

			if((*root)->children_size > 0)
			{
				for(int x = med + 1; x < DEG;x++)
				{
					new->children[new->children_size++] = (*root)->children[x];
				}

				new->children[new->children_size++] = extra_chd;
				(*root)->children_size = med + 1;
			}


			//new->children_size = DEG - (med + 1);
			*new_chd = new;
			//*pass = true;
			return to_ret;
		}
	}

	*pass = false;
	return 0;
}

void insert(Node **root, int value)
{
	printf("%d started\n", value);
	Node *new_chd = NULL;
	bool pass = false;
	int ret_val = insert_main(root, &pass, &new_chd, value);
	
	if(pass)
	{
		Node *new = NULL;
		MAKE_NEW(new);
		new->values[new->value_size++] = ret_val;
		new->children[0] = *root;
		new->children[1] = new_chd;
		new->children_size = 2;
		*root = new;
	}
	printf("%d ended\n\n", value);
}

int check_balance(Node *node, Node *new_node)
{
	return 0;
}

void preorder(Node *root)
{
	if(root == NULL)
		return;

	//printf("values: %d, children: %d\n", root->value_size, root->children_size);
	int i = 0;
	for(; i < root->value_size; i++)
	{
		if(root->children_size > 0)		
			preorder(root->children[i]);
		printf("%d\n", root->values[i]);
	}

	if(root->children_size > 0)
		preorder(root->children[i]);
	return;
}
