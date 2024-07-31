#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#define ISVALIDSOCKET(s) (s != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())
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
#define GETSOCKETERRNO() errno
#define SOCKET int

#endif

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
#if defined(_WIN32)
	WSAWORD d;
	if(WSAStartup(MAKEWORD(2, 2), &d)) {
		fprintf(stderr, "Failed to initialize\n");
		return 1;
	}
#endif

	printf("Welcome to socket world\n");
	struct addrinfo hints, *bind_info;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, "8080", &hints, &bind_info);
	/*printf("%d\n", getaddrinfo(NULL, "8080", &hints, &bind_info));
	printf("%s %d %d %d\n", bind_info->ai_family == AF_INET ? "IPv4" : "Invalid", bind_info->ai_socktype, bind_info->ai_protocol, bind_info->ai_addrlen);*/
	SOCKET sock = socket(bind_info->ai_family, bind_info->ai_socktype, bind_info->ai_protocol);

	if(!ISVALIDSOCKET(sock)) {
		perror("Cannot create the socket");
		exit(GETSOCKETERRNO());
	}

	if(bind(sock, bind_info->ai_addr, bind_info->ai_addrlen)) {
		perror("Cannot bind the address");
		exit(GETSOCKETERRNO());
	}

	if(listen(sock, 10) < 0) {
		perror("Cannot listen to the connections rn");
		exit(GETSOCKETERRNO());
	}

	char hostaddr[INET6_ADDRSTRLEN];
	memset(hostaddr, 0, sizeof(hostaddr));
	if(getnameinfo(bind_info->ai_addr, bind_info->ai_addrlen, hostaddr, sizeof(hostaddr), 0, 0, NI_NUMERICHOST)) {
		fprintf(stderr, "Unable to get hostname\n");
		return 1;
	}
	printf("Serving at %s:%s\n", strlen(hostaddr) ? hostaddr : "127.0.0.1", "8080");
	freeaddrinfo(bind_info);

	puts("Waiting for client connection...");

	while(1) {

		struct sockaddr_storage client;
		socklen_t client_size = sizeof(client);
		SOCKET  client_sock = accept(sock, (struct sockaddr *) &client, &client_size);

		if(!ISVALIDSOCKET(client_sock)) {
			perror("Accept error");
			exit(GETSOCKETERRNO());
		}

		char client_addr[INET6_ADDRSTRLEN];
		getnameinfo((struct sockaddr *) &client, client_size, client_addr, sizeof(client_addr), NULL, 0, NI_NUMERICHOST);
		printf("Client address: %s\n", client_addr);

		puts("Reading request...");
		char buff[1024];
		int received = recv(client_sock, buff, sizeof(buff), 0);
		printf("%.*s", received, buff);

		puts("Sending Response...");
		const char *response =
					"HTTP/1.1 200 OK\r\n"
					"Connection: close\r\n"
					"Content-Type: text/html\r\n\r\n"
					"<!DOCTYPE>"
					"<html lang='en'>"
					"<head>"
					"<meta charset='UTF-8'>"
					"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
					"<title>Current Time</title>"
					"</head>"
					"<style>"
					"h1 {"
					"text-align: center"
					"}"
					"</style>"
					"<body>"
					"<h1>";
		int bytes_sent = send(client_sock, response, strlen(response), 0);
		printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

		time_t t;
		if(time(&t) == (time_t) (-1)) {
			throw:
				perror("Unable to get the time\n");
				return 1;
		}

		char *curr_time = ctime(&t);
		printf("%s %d\n", curr_time, (int)strlen(curr_time));
		bytes_sent = send(client_sock, curr_time, strlen(curr_time), 0);
		printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(curr_time));

		const char *final =
					"</h1>"
					"</body>"
					"</html>";
		send(client_sock, final, strlen(final), 0);

		puts("Closing Connection...");
		CLOSESOCKET(client_sock);
	}
	puts("Closing listening to new connection...");
	CLOSESOCKET(sock);

#if defined(_WIN32)
	WSACleanup();
#endif
	return 0;
}
