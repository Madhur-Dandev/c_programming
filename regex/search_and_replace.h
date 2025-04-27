#ifndef _SRCH_AND_RPLC
#define _SRCH_AND_RPLC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

struct token_node
{
	int grp_no;
	char *str;
	struct token_node *next;
};

struct rplc_str_details
{
	struct token_node *ptr;
	int grps;
};

int show_error(int, regex_t *);
int get_no_of_grps(char *);
char *search_replace(char *, char *, char *, int);
void insert_node(struct token_node **, struct token_node **, int, char *, int);
void free_resource(struct token_node *);
struct rplc_str_details *gen_rplc_token(char *);

#endif
