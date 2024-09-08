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

struct del_ds {
	node *data_node;
	node *parent;
	struct del_ds *next;
};

int insert(node **, int);

int delete(node **);

void preorder(node *);

void inorder(node *);

void postorder(node *);

void preorder_no_recur(node *);

void postorder_no_recur(node *);

void levelorder(node *);

void reverse_levelorder(node *);

int max(node *);

int min(node *);

int max_no_recur(node *);

int min_no_recur(node *);

node *search(node *, int);

int size(node *);

int get_height(node *, int);

node *search_no_recur(node *, int);

int size_no_recur(node *);
