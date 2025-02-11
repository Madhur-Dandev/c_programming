// changes: find_parent function should return the parent's address
// 			instead of parent's structure
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct node
{
	int32_t value;
	struct node *left;
	struct node *right;
};

struct node *create_node(int32_t);
int32_t calculate_height(struct node *);
void check_balance(struct node **);
void insertion(struct node **, int32_t);
struct node **find_parent(struct node **, int32_t);
void rotate_left(struct node **);
void rotate_left_right(struct node **);
void rotate_right(struct node **);
void rotate_right_left(struct node **);
void delete_node(struct node **, int32_t);
void preorder(struct node *);
void inorder(struct node *);
void postorder(struct node *);
void destroy(struct node **);

int32_t main(void)
{
	struct node *root = NULL;
	/*insertion(&root, 10);
	insertion(&root, 20);
	insertion(&root, 15);*/
	//insertion(&root, 30);
	/*insertion(&root, 50);
	insertion(&root, 30);
	insertion(&root, 70);
	insertion(&root, 20);
	insertion(&root, 40);
	insertion(&root, 60);
	insertion(&root, 80);
	insertion(&root, 10);
	insertion(&root, 25);
	insertion(&root, 35);
	insertion(&root, 45);
	//insertion(&root, 36);
	//delete_node(&root, 45);
	//delete_node(&root, 40);
	//delete_node(&root, 30);
	delete_node(&root, 50);*/
	for(int32_t i = 0; i < 10; i++)
	{
		insertion(&root, 1);	
	}

	delete_node(&root, 3);
	delete_node(&root, 7);

	preorder(root);
	putc('\n', stdout);
	inorder(root);
	printf("%d\n", calculate_height(root->left) - calculate_height(root->right));
	destroy(&root);
	return 0;
}

struct node *create_node(int32_t value)
{
	struct node *new = (struct node *) malloc(sizeof(struct node));
	if(!new)
		return NULL;

	new->left = new->right = NULL;
	new->value = value;
	return new;
}

int32_t calculate_height(struct node *root)
{
	if(!root)
		return -1;

	int32_t left = calculate_height(root->left);
	int32_t right = calculate_height(root->right);

	return (left > right ? left : right) + 1;
}

void check_balance(struct node **root)
{
	if(!(*root))
		return;

	int32_t left_height, right_height, child_left_height, child_right_height;
	int32_t difference, child_difference;
	left_height = right_height = child_left_height = child_right_height = 0;

	left_height = calculate_height((*root)->left);
	right_height = calculate_height((*root)->right);

	difference = left_height - right_height;

	if(difference == 0 || difference == 1 || difference == -1)
		return;

	if(difference > 1)
	{
		child_left_height = calculate_height((*root)->left->left);
		child_right_height = calculate_height((*root)->left->right);
		child_difference = child_left_height - child_right_height;

		if(child_difference < 0)
			rotate_left_right(root);
		else
			rotate_right(root);
	}
	
	if(difference < -1)
	{
		child_left_height = calculate_height((*root)->right->left);
		child_right_height = calculate_height((*root)->right->right);
		child_difference = child_left_height - child_right_height;

		if(child_difference > 0)
			rotate_right_left(root);
		else
			rotate_left(root);
	}
	
	return;
}

void insertion(struct node **root, int32_t value)
{
	if(!(*root))
	{
		*root = create_node(value);
		return;
	}

	if((*root)->value == value)
		return;

	if((*root)->value > value && !((*root)->left))
	{
		struct node *new = create_node(value);
		if(!new)
			return;
	
		(*root)->left = new;
	}
	else if((*root)->value < value && !((*root)->right))
	{
		struct node *new = create_node(value);
		if(!new)
			return;
	
		(*root)->right = new;
	}
	else
	{
		insertion((*root)->value > value
				  ? &(*root)->left
				  : &(*root)->right, value);
	}

	check_balance(root);

	return;
}

struct node **find_parent(struct node **root, int32_t value)
{
        if(!root)
        	return NULL;
        
        if((*root)->value < value)
        {
            if((*root)->right)
            	return find_parent((&(*root)->right), value);
            else
                return root;
        }
        else
        {
            if((*root)->left)
                return find_parent(&((*root)->left), value);
            else
                return root;
        }
}

void rotate_left(struct node **root)
{
	struct node *temp = (*root)->right->left;
	(*root)->right->left = *root;
	*root = (*root)->right;
	(*root)->left->right = NULL;

	if(!temp)
		return;
	
	struct node **parent = find_parent(root, temp->value);
	if(temp->value > (*parent)->value)
		(*parent)->right = temp;
	else
		(*parent)->left = temp;
}

void rotate_right(struct node **root)
{
	struct node *temp = (*root)->left->right;
	(*root)->left->right = *root;
	*root = (*root)->left;
	(*root)->right->left = NULL;

	if(!temp)
		return;
	
	struct node **parent = find_parent(root, temp->value);
	if(temp->value > (*parent)->value)
		(*parent)->right = temp;
	else
		(*parent)->left = temp;
}

void rotate_left_right(struct node **root)
{
	rotate_left(&((*root)->left));
	rotate_right(root);
}

void rotate_right_left(struct node **root)
{
	rotate_right(&((*root)->right));
	rotate_left(root);
}

void delete_node(struct node **root, int32_t target)
{
	if(!(*root))
	{
		puts("No such element.");
		return;
	}

	if((*root)->value == target)
	{
		struct node *to_free = *root;
		if((*root)->left && (*root)->right)
		{
			*root = (*root)->left;
			struct node **parent = find_parent(root, to_free->right->value);
			(*parent)->right = to_free->right;
			check_balance(parent);
		}
		else if((*root)->left || (*root)->right)
		{
			*root = (*root)->left ? (*root)->left : (*root)->right;
		}
		else
		{
			*root = NULL;
		}
		free(to_free);
	}
	else
	{
		delete_node((*root)->value > target
					? &((*root)->left)
					: &((*root)->right), target);
	}

	check_balance(root);
	return;
}

void preorder(struct node *root)
{
	if(!root)
		return;

	printf("%d\n", root->value);
	preorder(root->left);
	preorder(root->right);
	
	return;
}

void inorder(struct node *root)
{
	if(!root)
		return;

	inorder(root->left);
	printf("%d\n", root->value);
	inorder(root->right);
	
	return;
}

void postorder(struct node *root)
{
	if(!root)
		return;

	inorder(root->left);
	inorder(root->right);
	printf("%d\n", root->value);

	return;
}

void destroy(struct node **root)
{
	if(!(*root))
		return;

	destroy(&((*root)->left));
	destroy(&((*root)->right));

	free(*root);
	*root = NULL;
}
