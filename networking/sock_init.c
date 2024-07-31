#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#define ISVALIDSOCKET(s) (s != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO(s) (WSAGetLastError())
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#define ISVALIDSOCKET(s) (s >= 0)
#define CLOSESOCKET(s) close(s)
#define GETSOCKETERRNO(s) errno
#define SOCKET int

#endif

#include <stdio.h>

int main(void) {
#if defined(_WIN32)
	WSAWORD d;
	if(WSAStartup(MAKEWORD(2, 2), &d)) {
		fprintf(stderr, "Failed to initialize\n");
		return 1;
	}
#endif

	printf("Welcome to socket world\n");

#if defined(_WIN32)
	WSACleanup();
#endif
	return 0;
}
