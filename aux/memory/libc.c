#include <aux/devel.h>
#include <aux/memory/libc.h>

#include <aux/complain.h>
#include <aux/memory/memory.h>
#include <aux/memory/system.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

struct Allocation {
    char magic[sizeof(IPSD_ALLOCATION_MAGIC) - 1];
    size_t size;
    const char *file;
    const char *func;
    int line;
    uint8_t data[];
};

static void record_metadata(
  struct Allocation *allocation,
  size_t size,
  const char *file,
  const char *func,
  int line
) {
    memcpy(allocation->magic, IPSD_ALLOCATION_MAGIC, sizeof(IPSD_ALLOCATION_MAGIC) - 1);
    allocation->size = size;
    allocation->file = file;
    allocation->func = func;
    allocation->line = line;
}

static void *
libc_malloc_wrapper(void *opaque, size_t size, const char *file, const char *func, int line) {
    struct Allocation *allocation = malloc(size + sizeof(struct Allocation));
    if (allocation == NULL) {
        ipsd_fatal("out of memory", "malloc: %s\norigin: %s:%d", strerror(errno), file, line);
        abort();
    }

    record_metadata(allocation, size, file, func, line);
    return (void *)((char *)allocation + sizeof(struct Allocation));
}

static void *libc_realloc_wrapper(
  void *opaque,
  void *ptr,
  size_t size,
  const char *file,
  const char *func,
  int line
) {
    /* Check if we're OK. */
    struct Allocation *allocation = ptr - sizeof(struct Allocation);

#ifdef IPSD_DEV
    ipsd_assert_pointer_validity_with_origin("realloc", ptr, allocation->magic, file, func, line);
#endif /* IPSD_DEV */

    /* Do the reallocation. */
    allocation = realloc(allocation, size + sizeof(struct Allocation));
    if (allocation == NULL) {
        ipsd_fatal("out of memory", "realloc: %s\norigin: %s:%d", strerror(errno), file, line);
        abort();
    }

    record_metadata(allocation, size, file, func, line);
    return (void *)((char *)allocation + sizeof(struct Allocation));
}

static void libc_free_wrapper(void *opaque, void *ptr, const char *file, const char *func, int line) {
    struct Allocation *allocation = ptr - sizeof(struct Allocation);

    /* Check to see if this is valid memory to free if we're developing. */
#ifdef IPSD_DEV
    ipsd_assert_pointer_validity_with_origin("free", allocation, allocation->magic, file, func, line);
#endif /* IPSD_DEV */

    free(allocation);
}

static void libc_info(void *opaque, void *ptr, struct ipsd_AllocationInfo *info) {
    struct Allocation *allocation = ptr - sizeof(struct Allocation);

    info->file = allocation->file;
    info->func = allocation->func;
    info->line = allocation->line;
    info->size = allocation->size;
}

static void libc_debug(void *opaque) {
    IPSD_UNIMPLEMENTED;
}

void ipsd_init_libc_allocator(
  struct IPSD_Allocator *allocator_interface,
  struct IPSD_LibcData *libc_allocator
) {
    allocator_interface->alloc = libc_malloc_wrapper;
    allocator_interface->realloc = libc_realloc_wrapper;
    allocator_interface->free = libc_free_wrapper;
    allocator_interface->info = libc_info;
    allocator_interface->debug = libc_debug;
    allocator_interface->opaque = libc_allocator;
}
