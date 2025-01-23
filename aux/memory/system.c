#include <aux/memory/system.h>

#include <aux/complain.h>
#include <aux/devel.h>
#include <aux/memory/libc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef IPSD_PLAT_MACH
#include <mach/mach.h>
#include <mach/mach_vm.h>
#endif /* IPSD_PLAT_MACH */

#ifdef IPSD_PLAT_APPLE
#include <malloc/malloc.h>
#endif /* IPSD_PLAT_APPLE */

#ifdef IPSD_PLAT_LINUX
#include <malloc.h>
#endif /* IPSD_PLAT_LINUX */

static size_t ipsd_memory_allocation_size(const void *ptr) {
    IPSD_DEVELOPMENT_ONLY;

    /* Support *BSD at some point. */
#if defined(IPSD_PLAT_APPLE)
    /* TODO: Figure out if `malloc_good_size` skews the results. */
    return malloc_good_size(malloc_size(ptr));
#elif defined(IPSD_PLAT_LINUX)
    return malloc_usable_size(ptr);
#else
    return -1;
#endif /* IPSD_PLAT_APPLE */
}

static bool ipsd_memory_is_owned(const void *ptr) {
    IPSD_DEVELOPMENT_ONLY;

    /* Support Linux and probably *BSD at some point. */
#ifdef IPSD_PLAT_MACH
    kern_return_t kr;
    mach_port_t task;
    vm_region_basic_info_data_t info;
    mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT_64;
    mach_vm_size_t size;
    mach_port_t object_name;
    mach_vm_address_t address = (mach_vm_address_t)ptr;

    task = mach_task_self();

    kr = mach_vm_region(
      task,
      &address,
      &size,
      VM_REGION_BASIC_INFO_64,
      (vm_region_info_t)&info,
      &info_count,
      &object_name
    );

    return kr == KERN_SUCCESS;
#else
    return true;
#endif
}

void ipsd_assert_pointer_validity_with_origin(
  const char *operation,
  const void *ptr,
  const char *magic,
  const char *file,
  const char *func,
  int line
) {
    IPSD_DEVELOPMENT_ONLY;

    if (ptr == NULL) {
        ipsd_fatal(operation, "tried to free NULL, origin: %s:%s:%d", file, func, line);
        abort();
    }

    if (ipsd_memory_is_owned(ptr) == false) {
        ipsd_fatal(operation, "memory not owned at %p, origin: %s:%s:%d", ptr, file, func, line);
        abort();
    }

    if (ipsd_memory_allocation_size(ptr) == 0) {
        ipsd_fatal(operation, "zero bytes allocated at %p, origin: %s:%s:%d", ptr, file, func, line);
        abort();
    }

    if (strncmp(magic, IPSD_ALLOCATION_MAGIC, sizeof(IPSD_ALLOCATION_MAGIC)) != 0) {
        ipsd_fatal(
          "bad free",
          "never allocated owned memory at %p (missing magic), origin: %s:%d",
          ptr,
          file,
          line
        );
        abort();
    }
}
