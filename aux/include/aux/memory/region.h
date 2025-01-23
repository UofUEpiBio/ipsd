#ifndef IPSD_AUX_MEMORY_REGION_H
#define IPSD_AUX_MEMORY_REGION_H

#include <aux/memory/libc.h>
#include <aux/memory/memory.h>

struct IPSD_RegionData {
    struct IPSD_Allocator *backing_allocator;

    void *start;
    void *end;
};

void ipsd_init_region_allocator(
  struct IPSD_Allocator *allocator_interface,
  struct IPSD_Allocator *backing_allocator,
  struct IPSD_RegionData *region_allocator
);

void ipsd_free_region(struct IPSD_Allocator *allocator);

#endif /* IPSD_INTERNAL_PKG_LIBEXTEND_MEMORY_REGION_H */
