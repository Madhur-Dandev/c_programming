#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <inttypes.h>

typedef struct tree_node node;
typedef struct tracker tracker;
typedef struct tree_node_list list;
typedef struct tree tree;

// values for determining sides of tree node
enum {
	LEFT = -1,	// left node
	ROOT = 0,	// root node, first node of tree
	RIGHT = 1	// right node
};

struct tree_node {
	int32_t val;
	node *left;
	node *right;
};

struct tracker {
	int32_t left;
	int32_t right;
};

struct tree_node_list {
	node *n;
	list *next;
};

struct tree {
	node *binary_tree;
	void (*insert_node) (tree *, int32_t);
	int32_t (*delete_node) (tree *);
	void (*destroy) (tree *);
};

tree *init_tree(void);
//node *find_node(node *, int32_t, int32_t, tracker *);
node *find_node(node *, int32_t *, int32_t *, int32_t *, int32_t);
//node *search_through_list(node *);
void insert_node(tree *, int32_t);
int32_t delete_node(tree *);
void preorder(node *);
void inorder(node *);
void postorder(node *);
void destroy_tree(node *);
void destroy(tree *);

#endif
