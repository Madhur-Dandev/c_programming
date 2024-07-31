#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define ISVALIDSOCKET(s) (s != INVALID_SOCKET)
#define GETSOCKETERROR() (WSAGetLastError())
#define CLOSESOCKET(s) (closesocket(s))

#else
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#define ISVALIDSOCKET(s) (s >= 0)
#define GETSOCKETERROR() (errno)
#define CLOSESOCKET(s) (close(s))
#define SOCKET int

#endif

#include <stdio.h>
#include <string.h>
