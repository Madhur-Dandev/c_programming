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

struct tracker *tracker;

struct node
{
	int value_size;
	int children_size;
	int values[DEG - 1];
	Node *children[DEG];
};

struct item
{
	int prnt;
	int idx;
	Node *chd;
	struct item *next;
};

struct tracker
{
	struct item *head;
};

#define MAKE_NEW(var) \
do { \
var = (Node *) malloc(sizeof(Node)); \
if(var == NULL) \
	perror("Unable to add new key"); \
\
} while(0);

void fix_error(Node *, int);
void insert(Node **, int);
void delete(Node **, int);
int check_balance(Node *, Node *);
void inorder(Node *);


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
	inorder(root);
	//delete(&root, 1);
	//delete(&root, 6);
	delete(&root, 99);
	//insert(&root, 0);
	inorder(root);
	return 0;
}

int track(int prnt, int idx, Node *chd)
{
	struct item *new = (struct item *) malloc(sizeof(struct item));
	if(new == NULL)
	{
		return 1;
	}
	memset(new, 0, sizeof(struct item));
	new->prnt = prnt;
	new->idx = idx;
	new->chd = chd;

	new->next = tracker->head;
	tracker->head = new;
	return 0;	
}

void free_tracker(struct tracker *tracker)
{
	struct item *temp = tracker->head, *to_free;
	while(temp != NULL)
	{
		printf("%d %d %p\n", temp->idx, temp->prnt, temp->chd);
		to_free = temp;
		temp = temp->next;
		free(to_free);
	}
	free(tracker);
}

void fix_error(Node *node, int init)
{
	if(node == NULL)
		return;

	node->values[node->value_size++] = tracker->head->prnt;
	
	int start_off = node->value_size;
	for(; node->value_size < DEG - 1;)
		node->values[node->value_size++] = tracker->head->chd->values[node->value_size - start_off];
	
	if(tracker->head->idx - 1 < DEG - 1)
	{
		for(int i = tracker->head->idx - 1; i < DEG - 2; i++)
			node->values[i]  = node->values[i + 1];
		node->values[DEG - 2] = tracker->head->chd->values[node->value_size - start_off];
	}

	start_off = node->children_size;
	if(node->children_size > 0)
	{
		for(; node->children_size > DEG;)
		{
			node->children[node->children_size++] = tracker->head->chd->children[node->children_size - start_off];
		}
		if(tracker->head->idx < DEG)
		{
			for(int i = tracker->head->idx; i < DEG - 1; i++)
				node->children[i]  = node->children[i + 1];
			node->children[DEG - 1] = tracker->head->chd->children[node->children_size - start_off];
		}
	}

	int idx = tracker->head->idx;
	free(tracker->head->chd);
	struct item *to_free = tracker->head;
	tracker->head = tracker->head->next;
	
	free(to_free);
	if(tracker->head == NULL)
		return;
	else
		fix_error(node->children[idx], 0);

	return;
}

int insert_main(Node **root, bool *pass, Node **new_chd, int value)
{
	if(*root == NULL)
	{
		Node *new;
		MAKE_NEW(new);
		if(new == NULL)
			return 0;
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
		//
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

			if(new == NULL)
			{
				for(; i < DEG - 2; i++)
					(*root)->values[i] = (*root)->values[i + 1];				
				if(i < DEG - 1)
					(*root)->values[DEG - 2] = extra;

				if(ret_chd != NULL)
				{
					for(; i < DEG - 1; i++)
						(*root)->children[i + 1] = (*root)->children[i + 2];
					if(i + 1 < DEG)
						(*root)->children[DEG - 1] = extra_chd;
				}

				fix_error((*root)->children[i], 0);
			}
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
			track(to_ret, i + 1, new);
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
	tracker = (struct tracker *) malloc(sizeof(struct tracker *));
	int ret_val = insert_main(root, &pass, &new_chd, value);

	if(pass)
	{
		Node *new = NULL;
		MAKE_NEW(new);
		if(new != NULL)
		{
			new->values[new->value_size++] = ret_val;
			new->children[0] = *root;
			new->children[1] = new_chd;
			new->children_size = 2;
			*root = new;
		}
	}
	
	printf("%p\n", tracker);
	free_tracker(tracker);
	printf("%d ended\n\n", value);
}

void left_shift(Node *node, int prnt_no)
{
	Node *lft_chd = node->children[prnt_no];
	Node *rht_chd = node->children[prnt_no + 1];

	lft_chd->values[lft_chd->value_size++] = node->values[prnt_no];
	node->values[prnt_no] = rht_chd->values[0];
	rht_chd->children_size--;
	for(int i = 0; i < rht_chd->value_size; i++)
		rht_chd->values[i] = rht_chd->values[i + 1];

	lft_chd->children[lft_chd->children_size++] = rht_chd->children[0];
	rht_chd->children_size--;
	for(int i = 0; i < rht_chd->children_size; i++)
		rht_chd->children[i] = rht_chd->children[i + 1];
	return;
}

void right_shift(Node *node, int prnt_no)
{
	Node *lft_chd = node->children[prnt_no];
	Node *rht_chd = node->children[prnt_no + 1];

	int extra = node->values[prnt_no], temp;
	for(int i = 0; i < rht_chd->value_size; i++)
	{
		temp = rht_chd->values[i];
		rht_chd->values[i] = extra;
		extra = temp;
	}
		
	rht_chd->values[rht_chd->value_size++] = extra;

	Node *xtr_chd = lft_chd->children[--(lft_chd->children_size)], *tmp_chd;
	for(int i = 0; i < rht_chd->children_size; i++)
	{
		tmp_chd = rht_chd->children[i];
		rht_chd->children[i] = xtr_chd;
		xtr_chd = tmp_chd;
	}

	rht_chd->children[rht_chd->children_size++] = xtr_chd;

	node->values[prnt_no] = lft_chd->values[--(lft_chd->value_size)];
	
	return;
}

bool delete_main(Node **node, int *ret_val, bool to_ret, int value)
{
	if(*node == NULL)
		return false;
	
	bool adjust = false;

	int j;
	if(to_ret)
	{
		if((*node)->children_size > 0)
		{
			adjust = delete_main(&((*node)->children[(*node)->children_size - 1]), ret_val, to_ret, 0);
			j = (*node)->value_size - 1;
		}
		else
		{
			*ret_val = (*node)->values[--((*node)->value_size)];
			//*ret_val = (*node)->values[(*node)->value_size - 1];
			return (*node)->value_size < ceil((double) (DEG - 2) / (double) 2) ? true : false; 
		}
	}
	else
	{
		int i = 0, j = (*node)->value_size - 1;
		bool found = false;
		while(i <= j)
		{
			int med = (i + j) / 2;
			if((*node)->values[med] > value)
				j = med - 1;
			else if((*node)->values[med] < value)
				i = med + 1;
			else
			{
				found = true;
				j = med;
				break;
			}
		}

		printf("%d %d\n", (*node)->values[0], j);
		if(found)
		{
			if((*node)->children_size > 0)
			{
				int new_val;
				adjust = delete_main(&((*node)->children[j - 1 >= 0 ? j - 1 : 0]), &new_val, true, 0);
				(*node)->values[j] = new_val;
				printf("Got left value %d\n", new_val);
			}
			else
			{
				--((*node)->value_size);
				for(int x = j; x < (*node)->value_size; x++)
					(*node)->values[x] = (*node)->values[x + 1];
				puts("Got the node with 0 child");
			}
		}
		else
		{
			if(j < 0)
				j = 0;
			adjust = delete_main(&((*node)->children[(*node)->values[j] < value? j + 1 : j]), ret_val, to_ret, value);
		}
	}

	if(adjust)
	{
		puts("adjust");
		int min = ceil((double) (DEG - 2) / (double) 2);

		bool merge = false;
		if((*node)->children[j]->value_size < min)
		{
			if(j > 0 && (*node)->children[j - 1]->value_size > min)
				right_shift(*node, j - 1);
			else if((*node)->children[j + 1]->value_size > min)
				left_shift(*node, j);
			else
				merge = true;
		}
		else
		{
			if((*node)->children[j + 1]->value_size > min)
				left_shift(*node, j);
			else
				merge = true;
		}

		if(merge)
		{
			Node *lft_chd = (*node)->children[j];
			Node *rht_chd = (*node)->children[j + 1];
			Node *prnt = *node;
			lft_chd->values[lft_chd->value_size++] = prnt->values[j];
			(*node)->value_size--;

			for(int x = j; x < (*node)->value_size; x++)
				prnt->values[x] = prnt->values[x + 1];

			for(int x = 0; x < rht_chd->value_size; x++)
				lft_chd->values[lft_chd->value_size++] = rht_chd->values[x];

			for(int x = 0; x < rht_chd->children_size; x++)
				lft_chd->children[lft_chd->children_size++] = rht_chd->children[x];

			free((*node)->children[j + 1]);
			(*node)->children_size--;
			
			for(int x = j + 1; x < prnt->children_size; x++)
				prnt->children[x] = prnt->children[x + 1];
		}

		adjust = (*node)->value_size < min ? true : false;
	}

	return adjust;
}

void delete(Node **root, int value)
{
	puts("Deletion in progess");
	if(delete_main(root, NULL, false, value))
	{
		Node *to_free = *root;
		*root = (*root)->children[0];
		free(to_free);
	}
	puts("Deletion ended");
	return;
}

int check_balance(Node *node, Node *new_node)
{
	return 0;
}

void inorder(Node *root)
{
	if(root == NULL)
		return;

	//printf("values: %d, children: %d\n", root->value_size, root->children_size);
	int i = 0;
	for(; i < root->value_size; i++)
	{
		if(root->children_size > 0)		
			inorder(root->children[i]);
		printf("%d\n", root->values[i]);
	}

	if(root->children_size > 0)
		inorder(root->children[i]);
	return;
}
