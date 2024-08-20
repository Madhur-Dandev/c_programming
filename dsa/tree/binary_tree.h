#include <stdlib.h>
#include <limits.h>

typedef struct tree_node {
	int data;
	struct tree_node *left;
	struct tree_node *right;
} node;

typedef struct ins_ll {
	node *data_node;
	struct ins_ll *next;
} lln;

struct tree_stack {
	node *data_node;
	struct tree_stack *prev;
	struct tree_stack *next;
};

node *root;

int insert(int);

void preorder(node *);

void inorder(node *);

void postorder(node *);

void preorder_no_recurr(node *);

void postorder_no_recurr(node *);

void levelorder(node *);

int max(node *);

int min(node *);

int max_no_recc(node *);

int min_no_recc(node *);

node *search(node *, int);

int size(node *);

void reverse_levelorder(node *);

int get_height(node *, int);
