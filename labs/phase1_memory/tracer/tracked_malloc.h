#ifndef TRACKED_MALLOC_H
#define TRACKED_MALLOC_H

#include <stddef.h>

void *tracked_malloc(size_t size);
void tracked_free(void *ptr);

#endif
