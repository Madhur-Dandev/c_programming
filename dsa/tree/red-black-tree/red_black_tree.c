#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdarg.h>

struct node
{
	uint32_t color : 1; // 0: black, 1: red
	int32_t value;
	struct node *left;
	struct node *right;
};

struct node **find_parent(struct node **, int32_t);
void fetch_height_and_black_height(struct node *, int32_t *, int32_t *, int32_t *); 
void rotate_left(struct node **);
void rotate_left_right(struct node **);
void rotate_right(struct node **);
void rotate_right_left(struct node **);
void flip_color(int32_t, ...);
void insert(struct node **, int32_t, bool); 
void insertion(struct node **, struct node **, int32_t);
void delete_node(struct node **, int32_t);
void preorder(struct node *);
void inorder(struct node *);
void postorder(struct node *);
void destroy(struct node **);

int main(void)
{
	struct node *root = NULL;
	/*insertion(&root, NULL, NULL, 10);
	insertion(&root, NULL, NULL, 20);
	insertion(&root, NULL, NULL, 30);
	insertion(&root, NULL, NULL, 40);
	insertion(&root, NULL, NULL, 50);
	insertion(&root, NULL, NULL, 60);
	insertion(&root, NULL, NULL, 70);
	insertion(&root, NULL, NULL, 80);*/
	insertion(&root, NULL, 11);
	insertion(&root, NULL, 28);
	insertion(&root, NULL, 1);
	insertion(&root, NULL, 2);
	insertion(&root, NULL, 5);
	insertion(&root, NULL, 98);
	insertion(&root, NULL, 3);
	insertion(&root, NULL, 3);
	insertion(&root, NULL, 41);
	// printf("%d %d %d %d\n", root->value, root->right->value, root->right->right->value, root->right->right->right->value);
	preorder(root);
	//inorder(root);
	destroy(&root);
	return 0;
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

void fetch_height_and_black_height(struct node *node, int32_t *height, int32_t *black_height, int32_t *difference)
{
	if(!node)
	{
		(*black_height) += 1;
		return;
	}

	int32_t lh_current, rh_current, lbh_current, rbh_current;
	lh_current = rh_current = lbh_current = rbh_current = 0;

	fetch_height_and_black_height(node->left, &lh_current, &lbh_current, NULL);
	fetch_height_and_black_height(node->right, &rh_current, &rbh_current, NULL);

	*black_height = lbh_current > rbh_current
					? lbh_current
					: rbh_current;
	*height = (lh_current > rh_current
			  ? lh_current
			  : rh_current) + 1;

	if(difference)
	{
		*difference = lh_current - rh_current;
	}

	if(!(node->color))
	{
		(*black_height) += 1;
	}

	return;
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

void flip_color(int32_t total, ...)
{
	struct node *node;
	va_list ap;
	va_start(ap, total);

	while(total)
	{
		node = va_arg(ap, struct node *);
		node->color = !(node->color);
		total--;
	}
	
	va_end(ap);
	return;
}

void insert(struct node **position, int32_t value, bool color)
{
	struct node *new = (struct node *) malloc(sizeof(struct node));
	new->color = color;
	new->value = value;
	new->left = new->right = NULL;
	*position = new;
	return;
}

void insertion(struct node **current, struct node **parent, int32_t value)
{
	if(*current && (*current)->value == value)
	{
		return;
	}
	else if(*current)
	{
		insertion((*current)->value > value
				  ? &((*current)->left)
				  : &((*current)->right), current, value);
	}
	else
	{
		insert(current, value, true);
		if(parent && (*parent)->color)
			flip_color(1, *parent);
		return;
	}
	
	// algorithm for checking balance & balancing

	if(parent != NULL)
	{
		int32_t left_height, right_height, left_b_height, right_b_height, l_child_diff, r_child_diff;
		left_height = right_height = left_b_height = right_b_height = l_child_diff = r_child_diff = 0;

		fetch_height_and_black_height((*current)->left, &left_height, &left_b_height, &l_child_diff);
		fetch_height_and_black_height((*current)->right, &right_height, &right_b_height, &r_child_diff);
	
		int32_t height_diff = left_height - right_height;
		int32_t b_height_diff = left_b_height - right_b_height;

		if((height_diff > 1 || height_diff < -1) && (b_height_diff != 0))
		{
			if((*current)->color)
			{
				if(height_diff > 1 && l_child_diff > 0)
				{
					rotate_right(current);
				}
				else if(height_diff < -1 && r_child_diff < 0)
				{
					rotate_left(current);
				}
				else if(height_diff > 1 && l_child_diff < 0)
				{
					flip_color(1, *current);
					rotate_left_right(current);
				}
				else
				{
					flip_color(1, *current);
					rotate_right_left(current);
				}
			}
			else
			{
				if(height_diff > 1 && l_child_diff > 0)
				{
					flip_color(1, *current);
					rotate_right(current);
				}
				else if(height_diff < -1 && r_child_diff < 0)
				{
					flip_color(1, *current);
					rotate_left(current);
				}
				else if(height_diff > 1 && l_child_diff < 0)
				{
					rotate_left_right(current);
				}
				else
				{
					rotate_right_left(current);
				}
			}
		}
		else if((height_diff <= 1 && height_diff >= -1) && b_height_diff != 0)
		{
			if(height_diff < 0)
			{
				if((*current)->left->color)
					flip_color(1, (*current)->left);
				else
					flip_color(1, (*current)->right);
			}
			
			if(height_diff > 0)
			{
				if((*current)->right->color)
					flip_color(1, (*current)->right);
				else
					flip_color(1, (*current)->left);
			}

			if((*current)->color)
			{
				if((*current)->left->color || (*current)->right->color)
					flip_color(1, *current);
			}
		}
		else if((height_diff > 1 && height_diff < -1) && b_height_diff == 0)

		{
			if(height_diff > 1 && l_child_diff > 0)
				rotate_right(current);
			else if(height_diff > 1 && l_child_diff < 0)
				rotate_left_right(current);
			else if(height_diff < -1 && r_child_diff < 0)
				rotate_left(current);
			else
				rotate_right_left(current);
		}
		else
		{		
			return;
		}
	}
	else
	{
		if((*current)->color)
			flip_color(1, *current);
	}
	/*if(!(*current))
	{
		insert(current, value, parent != NULL);
	}
	else
	{
		if((*current)->value == value)
			return;

		insertion((*current)->value > value ? &((*current)->left) : &((*current)->right), current, parent, value);
	}

	struct node *uncle = grandparent ? (*grandparent)->left == (*parent) ? (*grandparent)->right : (*grandparent)->left : NULL;
	if(!parent || (*parent)->color == false)
	{
		return;
	}
	else if(!grandparent && (*parent))
	{
		if((*parent)->color == true)
			((*parent)->color = false);
	}
	else if((*parent)->color && (uncle && uncle->color))
	{
		flip_color(3, *parent, *grandparent, (*grandparent)->left == (*parent)
											 ? (*grandparent)->right
											 : (*grandparent)->left);
	}
	else
	{
		flip_color(2, *parent, *grandparent);
		if((*grandparent)->left == *parent && (*parent)->left == *current)
		{
			// rotate right
			rotate_right(grandparent);
		}
		else if((*grandparent)->left == *parent && (*parent)->right == *current)
		{
			// rotate left-right
			rotate_left_right(grandparent);
		}
		else if((*grandparent)->right == *parent && (*parent)->left == *current)
		{
			// rotate right-left
			rotate_right_left(grandparent);
		}
		else
		{
			// rotate left
			rotate_left(grandparent);
		}
	}*/
}

void preorder(struct node *root)
{
	if(!root)
		return;

	printf("%d %d\n", root->value, root->color);
	preorder(root->left);
	preorder(root->right);
	
	return;
}

void inorder(struct node *root)
{
	if(!root)
		return;

	inorder(root->left);
	printf("%d %d\n", root->value, root->color);
	inorder(root->right);
	
	return;
}

void postorder(struct node *root)
{
	if(!root)
		return;

	inorder(root->left);
	inorder(root->right);
	printf("%d %d\n", root->value, root->color);

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
