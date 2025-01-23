#ifndef IPSD_AUX_MEMORY_LIBC_H
#define IPSD_AUX_MEMORY_LIBC_H

#include <aux/memory/memory.h>

/* Every allocation is marked with this. */
#define IPSD_ALLOCATION_MAGIC "IPSD"

struct IPSD_LibcData {
    /*
     * https://gcc.gnu.org/onlinedocs/gcc-4.4.7/gcc/Empty-Structures.html
     *
     * >  G++ treats empty structures as if they had a single member of type char.
     *
     * We'll be okay if we just emulate this behaviour.
     */
    char phantom;
};

void ipsd_init_libc_allocator(
  struct IPSD_Allocator *allocator_interface,
  struct IPSD_LibcData *libc_allocator
);

#endif /* IPSD_INTERNAL_PKG_LIBEXTEND_MEMORY_LIBC_H */
