#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

char *words[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninty", "eleven", "twelve", "thirteen", "forteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
int dest_spc = 100, dest_occup = 1;

void convert(unsigned long, uint8_t, uint8_t, char *);
void add_str(char **, char *);
int main(void) {
	unsigned long num = 11111111111111111111;
	char *final = (char *) malloc(sizeof(char)*dest_spc);
	convert(num, 0, 0, final);
	printf("%s\n", final);
	free(final);
	return 0;
}

void convert(unsigned long n, uint8_t curr, uint8_t skip_ten, char *buff) {
	if(curr%3 == 0)
		skip_ten = 0;
	char val[50] = "";
	if(n == 0)
		return;
	uint8_t num = n % 10;

	if(num == 0 || (curr%3 == 1 && skip_ten == 1))
		goto add;

	if(curr%3 == 0 && ((n/10)%10) == 1) {
		strcpy(val, words[(num-1)+9*2]);
		strcat(val, " ");
		skip_ten = 1;
		goto add;
	}

	if(curr%3 == 1 && !skip_ten) {
		strcat(val, words[(num-1)+9]);
		strcat(val, " ");
		goto exit;
	}
	strcpy(val, words[num-1]);
	strcat(val, " ");
	add:
		if(curr%3 == 2 && num != 0)
			strcat(val, "hundred ");
		switch(curr) {
			case 3:
				strcat(val, "thousand ");
				break;
			case 6:
				strcat(val, "million ");
				break;
			case 9:
				strcat(val, "billion ");
				break;
			case 12:
				strcat(val, "trillion ");
				break;
			case 15:
				strcat(val, "quadrillion ");
				break;
			case 18:
				strcat(val, "quintillion ");
				break;
			default:
				break;
		}
	exit:
		convert(n / 10, curr + 1, skip_ten, buff);
		add_str(&buff, val);
}

void add_str(char **dest, char *src) {
	while((dest_occup + strlen(src)) >= dest_spc) {
		*dest = realloc(*dest, dest_spc*2);
		dest_spc *= 2;
	}

	if(!strcmp(src, ""))
		return;
	strcat(*dest, src);
	dest_occup += strlen(src);
}
