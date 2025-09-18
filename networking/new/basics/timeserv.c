// problem in either client or server

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int print_err(int err)
{
	if(err != 0)
		errno = err;
	perror("Error");
	exit(EXIT_FAILURE);
}

int main(void)
{
	int listenfd, connfd;
	char buff[128];
	struct sockaddr_in sa;
	time_t t;

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		print_err(0);

	bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(9999);

	if(bind(listenfd, (struct sockaddr *) &sa, sizeof(sa)) < 0)
		print_err(0);

	puts("Server works");

	if(listen(listenfd, 5) < 0)
		print_err(0);

	for(;;)
	{
		if((connfd = accept(listenfd, NULL, NULL)) < 0)
			print_err(0);

		puts("Connection received");

		t = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&t));
		//write(connfd, buff, strlen(buff));

		printf("%lu\n", strlen(buff));
		for(int i = 0; i < strlen(buff); i++)
		{
			write(connfd, buff + i, 1);
		}

		if(close(connfd) < 0)
			print_err(0);	
	}

	if(close(listenfd) < 0)
		print_err(0);
	
	return 0;
}
