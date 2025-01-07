#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <inttypes.h>

typedef struct tree_node node;
typedef struct tracker tracker;
typedef struct tree_node_list list;
typedef struct tree tree;
typedef struct path_node path_node;

// values for determining sides of tree node
enum
{
	LEFT = -1, // left node
	ROOT = 0,  // root node, first node of tree
	RIGHT = 1  // right node
};

// node of a tree
struct tree_node
{
	int32_t val; // value of the node
	node *left;	 // address of left subtree/node
	node *right; // addrress of right subtree/node
};

// for making queue for level-order operations
struct tree_node_list
{
	node *n;	// address of the node in queue
	list *next; // address of next queue element.
};

// tree struct for efficiently managing tree resources.
struct tree
{
	node *binary_tree; // address of root node of tree
	int32_t height;	   // current height of tree
	void (*insert_node)(tree *, int32_t);
	int32_t (*delete_node)(tree *);
	void (*destroy)(tree *);
	path_node *(*find_path)(tree *, int32_t);
};

struct path_node
{
	char position;
	path_node *next;
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
node *search(tree *, int32_t);
path_node *find_path(tree *, int32_t);
void destroy_tree(node *);
void destroy(tree *);

#endif
