#include <stdio.h>
#include <regex.h>

char er_buf[128];

void match_email(regex_t *, char *);
int main(void) {
	char email_regex[] = "^[[:alnum:]_-.\"\']+@[[:alnum:]_-.]+\.[[:alnum:]]{2,4}\.[[:alnum:]]{2,4}$";	
	return 0;
}

void match_email(regex_t *pattern, char *str) {
	regmatch_t match;
	int res = regexec(pattern, str, 1, &match, REG_EXTENDED);

	if(res) {
		regerror(res, pattern, er_buf, sizeof(er_buf));
		fprintf(stderr, "Regex error: %s\n", er_buf);
	}
	printf("%lld %lld\n", match.rm_so, match.rm_eo);
	return;
}

