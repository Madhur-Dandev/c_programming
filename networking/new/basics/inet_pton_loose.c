#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

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
