#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <errno.h>
#include <string.h>

int main(void) {
	struct ifaddrs *addresses;

	if(getifaddrs(&addresses) == -1) {
		perror("Cannot get addresses");
	}

	struct ifaddrs *address = addresses;

	while(address) {
		int family = address->ifa_addr->sa_family;
		if(family == AF_INET || family == AF_INET6) {
			char ap[100], mask[100];
			if(address->ifa_netmask) {
				int netmask_family = address->ifa_netmask->sa_family;
				const int netmask_family_size = netmask_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
				getnameinfo(address->ifa_netmask, netmask_family_size, mask, sizeof(mask), NULL, 0, NI_NUMERICHOST);
			}
			printf("%s\n", address->ifa_name);
			printf("%s\n", family == AF_INET ? "IPv4" : "IPv6");
			const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
			getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap), NULL, 0, NI_NUMERICHOST);
			printf("\t%s\n", ap);
			if(strlen(mask))
				printf("\t%s\n", mask);
		}
		address = address->ifa_next;
	}

	freeifaddrs(addresses);

	return 0;
}
