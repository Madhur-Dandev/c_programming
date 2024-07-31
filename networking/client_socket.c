#include "starter.h"
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#endif

void terminate(char *);

int main(int argc, char **args) {
	if(argc < 3)
		terminate("usage: client_socket.exe host port\n");

#ifdef _WIN32
	WSAWORD word;
	if(WSAStartup(MAKEWORD(2, 2), &word)) {
		fprintf(stderr, "Error windows initialization\n");
		return 1;
	}
#endif

	struct addrinfo *server_info, hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(*(args+1), *(args+2), &hints, &server_info))
		terminate("DNS lookup failed\n");

	char address_buffer[100];
	char service_buffer[100];
	getnameinfo(server_info->ai_addr,
				server_info->ai_addrlen,
				address_buffer,
				sizeof(address_buffer),
				service_buffer,
				sizeof(service_buffer),
				NI_NUMERICHOST);
	printf("Server info:\n\tAddress: %s\n\tService: %s\n", address_buffer, service_buffer);

	printf("Creating socket\n");
	SOCKET client_sock = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if(!ISVALIDSOCKET(client_sock))
		terminate("Unable to create socket\n");

	if(connect(client_sock, server_info->ai_addr, server_info->ai_addrlen))
		terminate("Unable to connect to server\n");
	freeaddrinfo(server_info);

	printf("Connected!\nTo send data, enter text followed by enter.\n");

	while(1) {
		fd_set reads;
		FD_ZERO(&reads);
		FD_SET(client_sock, &reads);

#ifndef _WIN32
		/*
		 * add stdin to reads for non-windows system
		 */

		/* FD_SET(0, &reads); */
		/*
		 *or
		 */
		FD_SET(fileno(stdin), &reads);
#endif

		struct timeval time = {.tv_usec = 100000};
		if(select(client_sock+1, &reads, 0, 0, &time) < 0)
			terminate("select() failed!\n");
		if(FD_ISSET(client_sock, &reads)){
			char data_received[4096];
			int received = recv(client_sock, data_received, sizeof(data_received), 0);
			if(received < 1) {
				printf("Connection terminated by server.\n");
				break;
			}
			else
				printf("Data received: %d\nData: %.*s\n", received, received, data_received);
		}
#ifdef _WIN32
		if(_kbhit()) {
#else
		if(FD_ISSET(fileno(stdin), &reads)) {
#endif
			char read[4096];
			if(!fgets(read, sizeof(read), stdin)) break;
			printf("Sending read data...\n");
			int bytes_sent = send(client_sock, read, strlen(read), 0);
			printf("Bytes %d\n", bytes_sent);
		}
	}

	printf("Closing socket...\n");
	CLOSESOCKET(client_sock);

#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}

void terminate(char *msg) {
	fprintf(stderr,"%s", msg);
	exit(GETSOCKETERROR());
}
