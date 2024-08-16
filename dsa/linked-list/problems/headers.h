#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define TERMINATE(err) \
	{ \
		fprintf(stderr, "Error occured\n"); \
		exit(err); \
	}

#ifdef FORWARD_ONLY
typedef struct fo_node {
	int data;
	struct fo_node *next;
} fo_node;

#elif defined(DOUBLE)
typedef struct fb_node{
	int data;
	struct fb_node *prev;
	struct fb_node *next;
} fb_node;

#endif

#ifdef FORWARD_ONLY
#elif defined(DOUBLE)
void create_ll(fb_node **, fb_node **, int *);
void destroy_ll(fb_node **, fb_node **);
void insert(fb_node **, int, int *);
void delete(fb_node **, fb_node **, int *);
void print_list(fb_node **);
#endif
