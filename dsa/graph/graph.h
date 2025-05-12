#ifndef GRPH_H_
#define GRPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define VRTCS 4
#define CHECK_PARAM(a, b) \
if(a >= VRTCS || b >= VRTCS) \
{ \
	fprintf(stderr, "Invalid vertex\n"); \
	return; \
}

#endif
