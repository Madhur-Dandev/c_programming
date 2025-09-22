#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <regex.h>

#define MAX_ERRLEN 256
#define SA struct sockaddr

/* for ipv6 wildcard address */
#ifndef IN6ADDR_ANY
#	define IN6ADDR_ANY ::
#endif

#ifndef IN6ADDR_LOOPBACK
#	define IN6ADDR_LOOPBACK ::1
#endif

#define INADDRSTR_LOOPBACK "127.0.0.1"
#define INADDRSTR_ANY "0.0.0.0"

/* common way to handle an error */
#define COMMON_ERR_HANDLING(func) 							\
	if(func < 0)											\
		error_handlder(0, true);

/* Function macros for handling interrupt error */
#define COMMON_EINTR_HANDLING(res, func) 					\
	while((res = func) < 0) \
	{														\
		if(errno == EINTR) 									\
			continue; 										\
															\
		error_handlder(0, true); 									\
	}

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

void
error_handler(int, bool);

int
build_headers_regex(regex_t *);

int
main(void)
{
	int lissock, clisock;
	struct sockaddr_in lisaddr, cliaddr;
	
	uint32_t len;

	lissock = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	bzero(&lisaddr, sizeof(lisaddr));
	lisaddr.sin_family = AF_INET;
	lisaddr.sin_port = htons(80);
	if(inet_pton(AF_INET, INADDRSTR_ANY, &lisaddr.sin_addr) < 0)
		error_handlder(0, true);

	Bind(lissock, (SA *) &lisaddr, sizeof(lisaddr));

	Listen(lissock, 5);

	len = sizeof(cliaddr);

	/* using cliaddr for saving some memory */
	if(getsockname(lissock, (SA *) &cliaddr, &len) < 0)
		error_handlder(0, false);

	printf("Server started at: %s:%hu\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

	bzero(&cliaddr, sizeof(cliaddr));
	while((clisock = Accept(lissock, (SA *) &cliaddr, &len)))
	{
		printf("Client Connected: %s:%hu\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
		Close(clisock);
	}

	Close(lissock);
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
										address_len));

	return res;
}

int Close(int fildes)
{
	int res;

	COMMON_EINTR_HANDLING(res, close(fildes));

	return res;
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
		exit(EXIT_FAILURE);
}

int
build_headers_regex(regex_t *reg)
{
	static const char
	regpttrn[] = "^(accept|accept-encoding|allow|cache-\
				control|client-ip|connection|content-\
				encoding|content-type|content-lenght|\
				date|except|host|user-agent):.*($|\\n|\\r\\n)";

	int res;
	if((res = regcomp(reg, regpttrn, REG_EXTENDED|REG_ICASE)) != 0)
		error_handler(res, false);

	return res;
}
