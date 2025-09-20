#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_ERRLEN 256
#define SA struct sockaddr

void
error_handler(int);

int
main(void)
{
	
}

void
error_handler(int err)
{
	if(err != 0)
		errno = err;
	
	if(errno == 0)
		return;

	char err_msg[MAX_ERRLEN];

	if(errno == EACCES)
	{
		strncpy(err_msg, "Permission Denied", MAX_ERRLEN);
	}
	else if(errno == ENOMEM || errno == NOBUFS)
	{
		strncpy(err_msg, "Insufficient memory", MAX_ERRLEN);
	}
	else
	{
		strerror_r(errno, err_msg, MAX_ERRLEN);
	}
	fprintf(stderr, "Error: %s\n", err_msg); 
}
