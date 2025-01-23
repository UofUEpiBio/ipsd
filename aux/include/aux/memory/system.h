#ifndef IPSD_AUX_MEMORY_SYSTEM_H
#define IPSD_AUX_MEMORY_SYSTEM_H

#define ipsd_assert_pointer_validity(operation, ptr, magic)                                        \
    ipsd_assert_pointer_validity_with_origin(operation, ptr, magic, __FILE__, __func__, __LINE__);

void ipsd_assert_pointer_validity_with_origin(
  const char *operation,
  const void *ptr,
  const char *magic,
  const char *file,
  const char *func,
  int line
);

#endif /* IPSD_AUX_MEMORY_SYSTEM_H */
