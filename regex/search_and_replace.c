/* Error handling missing
 * High overhead
 */

#include "search_and_replace.h"

int main(void)
{
	char *str = search_replace("(madhur) (dandev)", "Hey! My name is madhur dandev", "{1} pradip {2}", REG_ICASE);
	puts(str);
	free(str);
	//printf("%d\n", get_no_of_grps("(madhur) (dandev)"));
	return 0;
}

int show_error(int err, regex_t *reg) {
	char e_buf[128];
	int n_read = regerror(err, reg, e_buf, sizeof(e_buf));
	if(n_read < 1) {
		printf("Cannot get the error string\n");
		return 1;
	}

	printf("Regex error: %s\n", e_buf);
	return err;
}

int get_no_of_grps(char *str)
{
	regex_t reg;
	int result;
	//if((result = regcomp(&reg, "([^\\])(\\(+)[^\\)]*([^\\])(\\)+)", REG_EXTENDED)))
	if((result = regcomp(&reg, "(^|[^\\])(\\(+)[[:alnum:][:punct:][:space:]]*([^\\])(\\)+)", REG_EXTENDED | REG_UNGREEDY | REG_ENHANCED)))
	{
		return show_error(result, &reg);
	}

	int start_offset = 0;
	int grps = 0;
	int str_len = strlen(str);
	regmatch_t locations[5];
	while(!regexec(&reg, str + start_offset, 5, locations, 0))
	{
		int opening_diff = locations[2].rm_eo - locations[2].rm_so;
		int closing_diff = locations[4].rm_eo - locations[4].rm_so;

		grps += (opening_diff > closing_diff ? closing_diff : opening_diff);
		char first[2];
		char last[2];
		strncpy(first, str + (start_offset + locations[1].rm_so), locations[1].rm_eo - locations[1].rm_so);
		strncpy(last, str + (start_offset + locations[3].rm_so), locations[3].rm_eo - locations[3].rm_so);
		if(first[0] == '(' && last[0] == ')')
		{
			grps++;
		}
		start_offset += locations[0].rm_eo;
		if(start_offset >= str_len)
			break;
	}
	regfree(&reg);
	return grps;
}

char *search_replace(char *regex, char *str, char *rplc_str, int flag)
{
	struct rplc_str_details *ptr = gen_rplc_token(rplc_str);
	struct token_node *temp = ptr->ptr;
	int total_grps = get_no_of_grps(regex);
	if(ptr->grps > total_grps)
	{
		fprintf(stderr, "Group index exceeded in replacement string.");
		free_resource(ptr->ptr);
		free(ptr);
		return NULL;
	}
	regex_t reg;
	regcomp(&reg, regex, REG_EXTENDED | REG_ICASE);
	int str_len = strlen(str);
	int final_str_len = 0;
	char *final_str = (char *) malloc(sizeof(char) * (str_len + 1));
	int start_offset;
	regmatch_t *locations = (regmatch_t *) calloc(ptr->grps + 1, sizeof(regmatch_t));

	while(!regexec(&reg, str, ptr->grps + 1, locations, 0))
	{
		int char_size = 128;
		char *main_str = (char *) calloc(char_size + 1, sizeof(char));
		int main_str_len = 0;
		struct token_node *temp = ptr->ptr;
		while(temp)
		{
			int node_str_size = 0;
			if(temp->grp_no == -1)
				node_str_size = strlen(temp->str);
			else
				node_str_size = locations[temp->grp_no].rm_eo - locations[temp->grp_no].rm_so;

			if(main_str_len + node_str_size > char_size)
			{
				char_size += node_str_size;
				main_str = realloc(main_str, sizeof(char) * char_size);
			}
			if(temp->grp_no == -1)
			{
				strcpy(main_str + main_str_len, temp->str);
			}
			else
			{
				strncpy(main_str + main_str_len, str + (start_offset + locations[temp->grp_no].rm_so), node_str_size);
			}
			main_str_len += node_str_size;
			temp = temp->next;
		}

		if(locations[0].rm_so != 0)
		{
			if(final_str_len + locations[0].rm_so > str_len)
			{
				str_len = final_str_len + locations[0].rm_so + 1;
				final_str = (char *) realloc(final_str, sizeof(char) * str_len);
			}
			strncpy(final_str + final_str_len, str + start_offset, locations[0].rm_so);
			final_str_len += locations[0].rm_so;
		}

		if(final_str_len + main_str_len > str_len)
		{
			int str_len = final_str_len + main_str_len + 1;
			//final_str = (char *) realloc(final_str, sizeof(char) * (final_str_len + main_str_len + 1));
			final_str = (char *) realloc(final_str, sizeof(char) * str_len);
		}
		strcpy(final_str + final_str_len, main_str);
		final_str_len += main_str_len;

		free(main_str);
		start_offset += locations[0].rm_eo;
		if(start_offset >= str_len)
			break;
	}
	regfree(&reg);
	free_resource(ptr->ptr);
	free(ptr);
	free(locations);
	return final_str;
}

void insert_node(struct token_node **head, struct token_node **tail, int grp_no, char *str, int size)
{
	struct token_node *new = (struct token_node *) malloc(sizeof(struct token_node));
	new->grp_no = grp_no;
	new->str = (char *) malloc(sizeof(char) * (size + 1));
	strncpy(new->str, str, size);
	if(*tail == NULL)
	{
		*head = *tail = new;
	}
	else
	{
		(*tail)->next = new;
		*tail = new;
	}
	return;
}

void free_resource(struct token_node *head)
{
	struct token_node *temp = head;
	while(head)
	{
		head = head->next;
		free(temp->str);
		free(temp);
		temp = head;
	}
	return;
}

struct rplc_str_details *gen_rplc_token(char *r_str)
{
	regex_t r_regex;
	if(regcomp(&r_regex, "(^|[^\\])\\{([[:digit:]]+)\\}", REG_EXTENDED))
	{
		printf("Compilation Error");
	}

	regmatch_t location[3];
	unsigned int r_str_len = strlen(r_str);
	int start_offset = 0;
	int max_grps = 0;
	struct token_node *head, *tail;
	head = tail = NULL;
	while(!regexec(&r_regex, r_str+start_offset, 3, location, 0))
	{
		char grp[(location[2].rm_eo - location[2].rm_so) + 1];
		strncpy(grp, r_str + (location[2].rm_so + start_offset), location[2].rm_eo - location[2].rm_so);
		if(location[1].rm_eo != 0)
		{
			insert_node(&head, &tail, -1, r_str+start_offset, location[1].rm_eo);
		}

		if(atoi(grp) > max_grps)
			max_grps = atoi(grp);
		insert_node(&head, &tail, atoi(grp), "", 0);
		start_offset = location[0].rm_eo + start_offset;
		if(start_offset >= r_str_len)
			break;
	}

	if(head == NULL)
		insert_node(&head, &tail, -1, r_str, strlen(r_str));
	if(start_offset < r_str_len)
		insert_node(&head, &tail, -1, r_str + start_offset, strlen(r_str) - start_offset);

	regfree(&r_regex);

	struct rplc_str_details *ptr = (struct rplc_str_details *) malloc(sizeof(struct rplc_str_details));
	ptr->ptr = head;
	ptr->grps = max_grps;
	return ptr;
}

