/* syntax: <scheme>://<user>:<password>@<host>:<port>/<path>;<params>?<query>#<frag>
 * Supported schemes: http, ftp, smtp, mailto, telnet, file
 */
#include <string.h>
#include <stdlib.h>
#include <regex.h>

// Error codes
#define EMALFRMURL 1
#define EMALFRMPATH 2
#define EMALFRMQURY 3

// Schemes
#define http 1
#define smtp 2
#define ftp 3

struct path
{
};

struct urltkn
{
	int scheme;
	char *user;
	char *pass;
	char *host;
	char *port;
	char *path;
	char *query;
	char *frag;
};

struct param
{
	int prmcount;
	char **key;
	char **value;
};

struct pathseg
{
	char *flefldname;
	struct param *psprms;
	struct pathseg *next;
};

void urltknfree(struct urltkn *urlinfo)
{
	
}

int parse(char *url)
{
	// checking and parsing will be divided into three
	// parts: hostname, path, others.
	regex_t reg;
	int ret;
	
	char errbuf[128];
	regmatch_t *match;
	struct urltkn *url_info ;
	/* urlregex breakdown
	 * scheme pattern: [A-z][A-z0-9+\\.-]+:// (optional)
	 * username and password: username = (([A-z0-9\\._@:-]+):([A-z0-9\\._@:-]+)@) ...?
	 * ...	[A-z0-9\\._@:-]+ ; password = [A-z0-9\\._@:-]+@ (optional)
	 * 
	 */
	// char *urlregex = "(([A-z][A-z0-9+\\.-]+)://)?(([A-z0-9\\._@-]+)(:([A-z0-9\\._@-]+)?)?@)?([0-9]{1,3}(\\.[0-9]{1,3}){3}|[A-z0-9][A-z0-9-]+[A-z0-9](\\.[A-z0-9][A-z0-9-]+[A-z0-9]){1,})(:[0-9]{1,5})?"; // v1
	/* Groups info
	 * 1. <scheme>://
	 * 2. <username>:<password>@
	 * 3. <username>
	 * 4. :<password>
	 * 5. <host-ip>|<host-domain>
	 * 6. <last-matched-ip-portion>
	 * 7. <last-matched-domain-portion>
	 * 8. port
	 */
	char *urlregex = "([A-z][A-z0-9+\\.-]+://)?(([A-z0-9\\._@!\\*'\\(\\),\\+$;\\?&=-]+)(:[A-z0-9\\._@!\\*'\\(\\),\\+$;\\?&=-]*)?@)?([0-9]{1,3}(\\.[0-9]{1,3}){3}|[A-z0-9][A-z0-9-]+[A-z0-9](\\.[A-z0-9][A-z0-9-]+[A-z0-9]){1,})(:[0-9]{1,5})?";

	if((ret = regcomp(&reg, hostname, REG_EXTENDED)))
	{
		regerror(ret, &reg, errbuf, sizeof(errbuf));
		puts(errbuf);
		return 1;
	}
	else
	{
		match = (regmatch_t *) calloc(9, sizeof(regmatch_t));
		if(match == NULL)
		{
			perror("Error");
			return 1;
		}

		if((ret = regexec(&reg, url, 9, match, 0)))
		{
			regerror(ret, &reg, errbuf, sizeof(errbuf));
			puts(errbuf);
			return 1;
		}
		else
		{
			url_info = (struct urltkn *) calloc(1, sizeof(struct urltkn));
			if(url_info == NULL)
				return 1;

			if(match[0].rm_so == -1 && match[0].rm_eo == -1)
			{
				fprintf(stderr, "Error: Malformed URL");
				return 1;
			}
			
			if(match[1].rm_so != -1)
			{
				
			}
		}
	}
	
	regfree(&reg);
}

int main(void)
{
	char *url1 = "http://google.com";
	char *url2 = "http://google.com";
	char *url3 = "http://google.com";
	char *url4 = "http://google.com";
	char *url5 = "http://google.com";
	char *url6 = "http://google.com";
	char *url7 = "http://google.com";
	char *url8 = "http://user123@192.168.0.3:5500/root-;madhur=madhur";

	printf("%lu\n", strlen(url8));

	regex_t urlrgx;
	int rtrncde;

	char *regex = "[[:alpha:]]{1,20}://([[:alnum:]]+(:[[:alnum:]]+)?@)?((www.)?[[:alpha:]][[:alnum:]\\-]+(\\.[[:alpha:]]+){1,3}|([[:digit:]]{1,3}.){3}[[:digit:]]{1,3})(:[[:digit:]]{1,5})?((/[[:alnum:]\\ \\-]*)(;[[:alnum:]\\ \\-]+\\=[[:alnum:]\\ \\-]+)*)*/?((\\?([[:alnum:]\\-\\ ]+\\=[[:alnum:]\\-\\ ]+)))?";
	//char *regex = "[[:alpha:]]{1,20}://([[:alnum:]]+(:[[:alnum:]]+)?@)?((www.)?[[:alpha:]][[:alnum:]\\-]+(\\.[[:alpha:]]+){1,3}|([[:digit:]]{1,3}.){3}[[:digit:]]{1,3})(:[[:digit:]]{1,5})?((/[[:alnum:]\\-\\ ]*)(;[[:alnum:]\\-\\ ]+\\=[[:alnum:]\\-\\ ]+)*)*/?((\\?([[:alnum:]\\-\\ ]+\\=[[:alnum:]\\-\\ ]+)(\\&[[:alnum:]\\-\\ ]+\\=[[:alnum:]\\-\\ ]+)*))?(#[[:alnum:]\\-]+)?";
	// ((\\?([[:alnum:]\\-\\ ]+\\=[[:alnum:]\\-\\ ]+)(\\&[[:alnum:]\\-\\ ]+\\=[[:alnum:]\\-\\ ]+)*))?(#[[:alnum:]\\-]+)?
	regcomp(&urlrgx, regex, REG_EXTENDED);
	regmatch_t match;
	if((rtrncde = regexec(&urlrgx, url8, 1, &match, 0)))
		puts("unsuccess");
	else
		puts("success");

	printf("%lld %lld\n", match.rm_so, match.rm_eo);

	return 0;
}
