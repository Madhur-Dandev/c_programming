#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <limits.h>

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
	int32_t value; // value of the node
	node *left;	 // address of left subtree/node
	node *right; // addrress of right subtree/node
};

// for making queue for level-order operations
struct tree_node_list
{
	node *n;	// address of the node in queue
	list *next; // address of next element in the list.
	list *prev; // address of previous element in the list.
};

// tree struct for efficiently managing tree resources.
struct tree
{
	node *binary_tree; // address of root node of tree
	void (*insert_node)(tree *, int32_t);
#ifdef BT
	void (*delete_node)(tree *);
#else
	void (*delete_node)(tree *, int32_t value);
#endif
	int32_t (*get_height)(tree *);
	int32_t (*get_node_level)(tree *, int32_t);
	void (*preorder)(tree *);
	void (*inorder)(tree *);
	void (*postorder)(tree *);
	void (*level_order)(tree *);
	int32_t (*find_smallest)(tree *);
	int32_t (*find_largest)(tree *);
	node *(*search)(tree *, int32_t);
	path_node *(*find_path)(tree *, int32_t);
	void (*destroy)(tree *);
};

struct path_node
{
	int32_t position;
	path_node *next;
};

tree *init_tree(void);
node *build_node(int32_t);
list *create_list_node(node *);
void clear_list(list *);
void insert_node(tree *, int32_t);
//node *find_parent_node(node *, int32_t *, int32_t *, int32_t *, int32_t);
#ifdef BT
void delete_node(tree *);
#else
void delete_node(tree *, int32_t);
#endif
int32_t get_height_main(node *);
int32_t get_height(tree *);
int32_t get_node_level_main(node *, int32_t);
int32_t get_node_level(tree *, int32_t);
void preorder_main(node *);
void inorder_main(node *);
void postorder_main(node *);
void preorder(tree *);
void inorder(tree *);
void postorder(tree *);
void level_order(tree *);
int32_t find_smallest_main(node *);
int32_t find_largest_main(node *);
int32_t find_smallest(tree *);
int32_t find_largest(tree *);
node *search_main(node *, int32_t);
node *search(tree *, int32_t);
path_node *make_path_node(path_node *, int32_t);
path_node *find_path(tree *, int32_t);
int32_t destroy_tree(node *);
void destroy(tree *);

#endif
