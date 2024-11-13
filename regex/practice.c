#include <stdio.h>
#include <regex.h>

int main(void) {
	regex_t reg;
	int res;
	char regex[] = "^[[:alpha:]]+";
//	char regex[] = "^[[:alpha:]]*";
//	char regex[] = "\\\\\"[[:alpha:]]*\\\\\"";
	char str[] = "alpha \\\"is\\\"lbeta";
	char er_buf[128];
	regmatch_t match;

	printf("%s\n", regex);

	res = regcomp(&reg, regex, REG_ICASE);
	if(res) {
		regerror(res, &reg, er_buf, sizeof(er_buf));
		fprintf(stderr, "Regex error: %s\n", er_buf);
		return res;
	}

	res = regexec(&reg, str, 1, &match, 0);
	if(res) {
		regerror(res, &reg, er_buf, sizeof(er_buf));
		fprintf(stderr, "Regex error: %s\n", er_buf);
		return res;
	}

	printf("%lld %lld\n", match.rm_so, match.rm_eo);
	return 0;
}
