#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>

int print_err(int err)
{
	if(err != 0)
		errno = err;

	printf("%d\n", errno);
	perror("Error");
	exit(EXIT_FAILURE);
}

int main(int argc, char *arg[])
{
	if(argc < 2)
		print_err(EINVAL);

	int clientfd;
	char recv_buff[128];
	struct sockaddr_in sa;

	if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		print_err(0);

	bzero(&sa, sizeof(sa));
	sa.sin_port = htons(9999);
	sa.sin_family = AF_INET;
	if(inet_pton(AF_INET, arg[1], &sa.sin_addr) < 0)
		print_err(0);

	if(connect(clientfd, (struct sockaddr *) &sa, sizeof(sa)) < 0)
		print_err(0);

	int n, count = 0;
	while((n = read(clientfd, recv_buff, sizeof(recv_buff))) > 0)
	{
		count++;
		recv_buff[n] = 0;
		if(fputs(recv_buff, stdout) == EOF)
			print_err(0);
	}
	printf("Count %d\n", count);

	if(n < 0)
	{
		fprintf(stderr, "Unable to read from socket\n");
		exit(EXIT_FAILURE);
	}

	if(close(clientfd) < 0)
		print_err(0);
	return 0;
}
