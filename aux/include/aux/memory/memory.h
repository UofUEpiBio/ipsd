#ifndef IPSD_AUX_MEMORY_MEMORY_H
#define IPSD_AUX_MEMORY_MEMORY_H

#include <stddef.h>

/* A shortcut for all the "tracer parameters" that are passed into allocation
 * functions.
 */
#define IPSD_ALLOCATION_TRACER_PARAMETERS const char *file, const char *func, int line

/* Allocation function dispatch. */
#define ipsd_alloc(allocator, size)                                                                \
    (allocator)->alloc((allocator)->opaque, size, __FILE__, __func__, __LINE__)

/* Reallocation function dispatch. */
#define ipsd_realloc(allocator, ptr, size)                                                         \
    (allocator)->realloc((allocator)->opaque, ptr, size, __FILE__, __func__, __LINE__)

/* Free function dispatch. */
#define ipsd_free(allocator, ptr)                                                                  \
    (allocator)->free((allocator)->opaque, ptr, __FILE__, __func__, __LINE__)

/* Generic information about an allocation. */
struct ipsd_AllocationInfo {
    const char *file;
    const char *func;
    int line;
    size_t size;
};

/* Allocation interface. */
struct IPSD_Allocator {
    void *(*alloc)(void *opaque, size_t size, IPSD_ALLOCATION_TRACER_PARAMETERS);
    void *(*realloc)(void *opaque, void *ptr, size_t size, IPSD_ALLOCATION_TRACER_PARAMETERS);
    void (*free)(void *opaque, void *ptr, IPSD_ALLOCATION_TRACER_PARAMETERS);
    void (*info)(void *opaque, void *ptr, struct ipsd_AllocationInfo *info);
    void (*debug)(void *opaque);
    void *opaque;
};

void ipsd_invalid_memory_operation();

#endif /* IPSD_AUX_MEMORY_MEMORY_H */
