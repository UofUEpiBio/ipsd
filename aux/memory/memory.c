#include <aux/memory/memory.h>
#include <aux/complain.h>

extern void abort(void);

void ipsd_invalid_memory_operation() {
    ipsd_fatal(
      "invalid memory operation",
      "the requested operation was rejected by the allocator in question"
    );
    abort();
}
