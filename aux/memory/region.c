#include <aux/memory/region.h>

#include <aux/memory/libc.h>
#include <aux/memory/memory.h>
#include <stdint.h>
#include <stdio.h>

struct Allocation {
    void *next_allocation;
    void *previous_allocation;
    uint8_t data[];
};

static void *region_alloc(void *opaque, size_t size, IPSD_ALLOCATION_TRACER_PARAMETERS) {
    struct IPSD_RegionData *region = opaque;
    struct IPSD_Allocator *backing_allocator = region->backing_allocator;

    /* No allocations, yet. */
    if (region->end == NULL) {
        /* Allocate. */
        struct Allocation *allocation
          = backing_allocator
              ->alloc(backing_allocator->opaque, size + sizeof(struct Allocation), file, func, line);

        /* Initialize the parent and child chain. */
        region->start = allocation;
        region->end = allocation;
        allocation->next_allocation = NULL;
        allocation->previous_allocation = NULL;

        /* Return a user-accessible pointer. */
        return allocation->data;
    }

    /* Allocate. */
    struct Allocation *allocation
      = backing_allocator
          ->alloc(backing_allocator->opaque, size + sizeof(struct Allocation), file, func, line);

    /* Append to the parent and child chain. */
    ((struct Allocation *)region->end)->next_allocation = allocation;
    allocation->previous_allocation = region->end;
    allocation->next_allocation = NULL;
    region->end = allocation;

    /* Return a user-accessible pointer. */
    return allocation->data;
}

static void *region_realloc(void *opaque, void *ptr, size_t size, IPSD_ALLOCATION_TRACER_PARAMETERS) {
    struct IPSD_RegionData *region = opaque;
    struct IPSD_Allocator *backing_allocator = region->backing_allocator;

    /* Handle NULL pointer input as a new allocation. */
    if (!ptr) {
        return region_alloc(opaque, size, file, func, line);
    }

    /* Store parent, child, and positioning information. */
    struct Allocation *old_allocation
      = (struct Allocation *)((uint8_t *)ptr - sizeof(struct Allocation));
    struct Allocation *parent_allocation = old_allocation->previous_allocation;
    struct Allocation *child_allocation = old_allocation->next_allocation;

    /* Reallocate. */
    struct Allocation *new_allocation = backing_allocator->realloc(
      backing_allocator->opaque, old_allocation, size + sizeof(struct Allocation), file, func, line
    );

    /* Set allocator references if we're the head or tail of the chain. */
    if (region->start == old_allocation) {
        region->start = new_allocation;
    }

    if (region->end == old_allocation) {
        region->end = new_allocation;
    }

    /* Reconnect parent and child links. */
    if (parent_allocation) {
        parent_allocation->next_allocation = new_allocation;
        new_allocation->previous_allocation = parent_allocation;
    } else {
        new_allocation->previous_allocation = NULL;
    }

    if (child_allocation) {
        child_allocation->previous_allocation = new_allocation;
        new_allocation->next_allocation = child_allocation;
    } else {
        new_allocation->next_allocation = NULL;
    }

    /* Return a user-accessible pointer. */
    return new_allocation->data;
}

static void region_info(void *opaque, void *ptr, struct ipsd_AllocationInfo *info) {
    struct IPSD_RegionData *region = opaque;
    struct IPSD_Allocator *backing_allocator = region->backing_allocator;
    struct Allocation *allocation
      = (struct Allocation *)((uint8_t *)ptr - sizeof(struct Allocation));

    backing_allocator->info(backing_allocator->opaque, allocation, info);
}

static void region_debug(void *opaque) {
    struct IPSD_RegionData *region = opaque;
    struct IPSD_Allocator *backing_allocator = region->backing_allocator;

    fprintf(
      stderr,
      "summary of memory region at %p (start=%p, end=%p):\n",
      region,
      region->start,
      region->end
    );

    /* Loop through every allocation in the list, showing it off. */
    struct Allocation *this_allocation = region->start;
    while (this_allocation != NULL) {
        struct ipsd_AllocationInfo allocation_info;
        size_t adjusted_size;

        backing_allocator->info(backing_allocator->opaque, this_allocation, &allocation_info);
        adjusted_size = allocation_info.size - sizeof(struct Allocation);

        fprintf(stderr, "\tallocation: %p: ", this_allocation);
        fprintf(
          stderr,
          "%s:%s:%d (%zu bytes)\n",
          allocation_info.file,
          allocation_info.func,
          allocation_info.line,
          adjusted_size
        );

        this_allocation = this_allocation->next_allocation;
    }
}

void ipsd_init_region_allocator(
  struct IPSD_Allocator *allocator_interface,
  struct IPSD_Allocator *backing_allocator,
  struct IPSD_RegionData *region_allocator
) {
    region_allocator->start = NULL;
    region_allocator->end = NULL;
    region_allocator->backing_allocator = backing_allocator;

    allocator_interface->alloc = region_alloc;
    allocator_interface->realloc = region_realloc;
    allocator_interface->free = ipsd_invalid_memory_operation;
    allocator_interface->info = region_info;
    allocator_interface->debug = region_debug;
    allocator_interface->opaque = region_allocator;
}

void ipsd_free_region(struct IPSD_Allocator *allocator) {
    struct IPSD_RegionData *region = allocator->opaque;

    /* Don't do anything if we haven't made any allocations. */
    if (region->start == NULL) {
        return;
    }

    /* Loop through every allocation in the list, killing it. */
    struct Allocation *this_allocation = region->start;
    for (;;) {
        struct Allocation *next_allocation;

        /* Have we freed everything? */
        if (this_allocation == NULL) {
            break;
        }

        next_allocation = this_allocation->next_allocation;
        ipsd_free(region->backing_allocator, this_allocation);
        this_allocation = next_allocation;
    }
}
