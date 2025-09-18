#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int
inet_pton_loose(int, const char *, void *);

int
main(void)
{
	/*long first_name = 0x000072756862614d;
	long middle_name = 0x0000706964617250;
	long last_name = 0x00007665646e6144;*/

	pid_t pid;

	fputs("Parent process starts\n", stdout);
	if((pid = fork()) == 0)
	{
		fputs("Child process starts\n", stdout);
		fputs("Child process ends\n", stdout);
	}
	else
	{
		nanosleep(&((struct timespec) {.tv_sec = 5}), NULL);
		fputs("Parent process ends\n", stdout);
	}
	return 0;
}

int
inet_pton_loose(int af, const char *src, void *dst)
{
	static int res;
	switch(af)
	{
		case AF_INET: {
			if((res = inet_pton(af, src, dst)) == 0)
				return inet_aton(src, dst); // return 1 or 0 for success or failure, respectively
			else if(res < 0)
				return -1; // errno will be set by inet_pton

			return res;
		}

		case AF_INET6: {
			struct in_addr temp;
			if((res = inet_pton(af, src, dst)) == 0)
			{
				if(inet_aton(src, &temp) == 1)
				{
					char ipv6[INET6_ADDRSTRLEN] = "::FFFF:";
					strncat(ipv6, src, strlen(src)); // src must be '\0' null terminated or the behavior is undefined.
					puts(ipv6);
					return inet_pton(af, ipv6, dst);
				}
				return 0;
			}

			return res;
		}

		default:
			errno = EAFNOSUPPORT;
			return -1;
	}
}
