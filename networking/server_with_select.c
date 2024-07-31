#include "starter.h"
#include <sys/select.h>

int main(void) {
#ifdef _WIN32
	WSAWORD word;
	if(WSAStartup(MAKEWORD(2, 2), &word)) {
		perror("Windows initialization error");
		return GETSOCKETERROR();
	}
#else

#endif
	return 0;
}
