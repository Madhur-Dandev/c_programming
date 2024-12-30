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
	int32_t height;
	void (*insert_node) (tree *, int32_t);
	int32_t (*delete_node) (tree *);
	void (*destroy) (tree *);
};

tree *init_tree(void);
node *find_node(node *, int32_t *, int32_t *, int32_t *, int32_t);
void insert_node(tree *, int32_t);
node *find_parent_node(node *, int32_t *, int32_t *, int32_t *, int32_t);
int32_t delete_node(tree *);
void preorder(node *);
void inorder(node *);
void postorder(node *);
void level_order(tree *);
int32_t find_smallest(node *);
int32_t find_largest(node *);
void destroy_tree(node *);
void destroy(tree *);

#endif
