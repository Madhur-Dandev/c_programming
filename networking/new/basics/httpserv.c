#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <regex.h>
#include <stdbool.h>

#define MAX_ERRLEN 256
#define SA struct sockaddr

/* for ipv6 wildcard address */
#ifndef IN6ADDR_ANY
#	define IN6ADDR_ANY ::
#endif

#ifndef IN6ADDR_LOOPBACK
#	define IN6ADDR_LOOPBACK ::1
#endif

#define INADDRSTR_LOOPBACK "127.0.0.1" 						/* String form of INADDR_LOOPBACK */
#define INADDRSTR_ANY "0.0.0.0"								/* String form of INADDR_ANY */

#define MAX_BUFF 4096 										/* maximum character buffer size */
#define HNAME_MAX 128 										/* max size of header name */
#define HVALUE_MAX 256

/* http version */
#define HTTPV1 	 1
#define HTTPV1_1 2
#define HTTPV2   3

/* http methods */
#define GET    	1
#define POST   	2
#define PUT	   	3
#define DELETE 	4
#define HEAD   	5
#define OPTIONS 6

/* structure for storing headers and header values */
struct header 
{
	char name[HNAME_MAX];
	char **value;
};

/* common way to handle an error */
#define COMMON_ERR_HANDLING(func) 							\
	if(func < 0)											\
		error_handler(0, true);

/* Function macros for handling interrupt error */
#define COMMON_EINTR_HANDLING(res, func, exit) 				\
	while((res = func) < 0) \
	{														\
		if(errno == EINTR) 									\
			continue; 										\
															\
		if(exit)											\
			error_handler(0, exit); 						\
		else												\
			return res;										\
	}

struct reqinfo 
{
	uint16_t method;
	uint16_t httpv;
	char path[512];
	
};

static regex_t reg_allwdhdrs;
static char pool[MAX_BUFF];
static int lastc = 0, pcount = 0;

int
Socket(int, int, int);

int
Bind(int, const SA *, socklen_t);

int
Listen(int, int);

int
Accept(int, SA *restrict, socklen_t *restrict);

int
Close(int);

ssize_t
readline(int, void *, size_t, void *, size_t, int *, int *);

void
error_handler(int, bool);

void
build_headers_regex(void);



int
main(void)
{
	int lissock, clisock;
	struct sockaddr_in lisaddr, cliaddr;
	
	uint32_t len;

	build_headers_regex();
		
	lissock = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	bzero(&lisaddr, sizeof(lisaddr));
	lisaddr.sin_family = AF_INET;
	lisaddr.sin_port = htons(80);
	if(inet_pton(AF_INET, INADDRSTR_ANY, &lisaddr.sin_addr) < 0)
		error_handler(0, true);

	Bind(lissock, (SA *) &lisaddr, sizeof(lisaddr));

	Listen(lissock, 5);

	len = sizeof(cliaddr);

	/* using cliaddr for saving some memory */
	if(getsockname(lissock, (SA *) &cliaddr, &len) < 0)
		error_handler(0, false);

	printf("Server started at: %s:%hu\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

	bzero(&cliaddr, sizeof(cliaddr));

	char buf[MAX_BUFF];
	ssize_t total_read;
	while((clisock = Accept(lissock, (SA *) &cliaddr, &len)))
	{
		pcount = lastc = 0;
		printf("Client Connected: %s:%hu\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
			
		while((total_read = readline(clisock, buf, MAX_BUFF, pool, MAX_BUFF, &pcount, &lastc)) > 0)
		{
			if(regexec(&reg_allwdhdrs, buf, 0, NULL, 0) == 0)
				puts(buf);
		}

		Close(clisock);
		puts("Connection terminated");
	}

	Close(lissock);
	regfree(&reg_allwdhdrs);
}

int
Socket(int af, int type, int protocol)
{
	int res;

	COMMON_ERR_HANDLING((res = socket(af, type, protocol)));

	return res;
}

int
Bind(int socket, const SA *address,
	socklen_t address_len)
{
	COMMON_ERR_HANDLING(bind(socket, address, address_len));

	return 0;
}

int
Listen(int socket, int backlog)
{
	COMMON_ERR_HANDLING(listen(socket, 5));

	return 0;
}

int
Accept(int socket, SA *restrict address,
		socklen_t *restrict address_len)
{
	int res;

	COMMON_EINTR_HANDLING(res, accept(socket, address,
								address_len), true);

	return res;
}

int Close(int fildes)
{
	int res;

	COMMON_EINTR_HANDLING(res, close(fildes), true);

	return res;
}

ssize_t readline(int fd, void *buf, size_t size, void *pool, size_t psize, int *ptotal, int *plastc)
{
	int count = 0;
	char *buffer = (char *) buf;
	char *pbuffer = (char *) pool;

	while(true)
	{
		if(*ptotal == 0)
		{
			COMMON_EINTR_HANDLING(*ptotal, read(fd, pool, psize), false);
			if(*ptotal == 0)
			{
				*ptotal = *plastc = 0;
				break;
			}
		}
		
		for(; *plastc < *ptotal; (*plastc)++)
		{
			if(pbuffer[*plastc] == '\n')
			{
				buffer[count] = 0;
				(*plastc)++;
				return count;
			}

			if(pbuffer[*plastc] == '\r')	
				continue;

			if(pbuffer[*plastc] == 0)
			{
				*ptotal = 0;
				break;
			}

			if(count < size)
				buffer[count++] = pbuffer[*plastc];
		}
	}
	return count;
}

void
error_handler(int err, bool to_exit)
{
	if(err != 0)
		errno = err;
	
	if(errno == 0)
		return;

	char err_msg[MAX_ERRLEN];

	/* Simplifying some messages */
	if(errno == EACCES)
	{
		strncpy(err_msg, "Permission Denied", MAX_ERRLEN);
	}
	else if(errno == ENOMEM || errno == ENOBUFS)
	{
		strncpy(err_msg, "Insufficient memory", MAX_ERRLEN);
	}
	else
	{
		strerror_r(errno, err_msg, MAX_ERRLEN);
	}
	fprintf(stderr, "Error: %s\n", err_msg);

	/* check whether to exit on failure */
	if(to_exit)
	{
		/* call to cleanup function here*/
		exit(EXIT_FAILURE);
	}
}

void
build_headers_regex(void)
{
	const char
	regpttrn[] = "^(accept|accept-encoding|allow|cache-\
				control|client-ip|connection|content-\
				encoding|content-type|content-lenght|\
				date|except|host|user-agent)[\t ]*:[\t ]*.*($|\\n|\\r\\n)";

	char err_buf[128];
	int res;
	if((res = regcomp(&reg_allwdhdrs, regpttrn, REG_EXTENDED|REG_ICASE)) != 0)
	{
		regerror(res, &reg_allwdhdrs, err_buf, 128);
		fprintf(stderr, "%s\n", err_buf);
	}
}
