#include <stdio.h>
#include <regex.h>

int show_error(int, regex_t *);

int main(void) {
	regex_t reg;

	char pattern[] = "s.ck";

	regmatch_t offsets;

	int res = regcomp(&reg, pattern, REG_ICASE | REG_NEWLINE);

	if(res)
		return show_error(res, &reg);

	char str[] = "sick, suck.";

	res = regexec(&reg, str, 1, &offsets, 0);

	if(res)
		return show_error(res, &reg);

	printf("%ld, %ld\n", offsets.rm_so, offsets.rm_eo);

	regfree(&reg);
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
