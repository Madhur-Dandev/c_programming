#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#define THROW_ERROR(err, msg) { \
fprintf(stderr, "%s\n", msg); \
exit(err); \
}
