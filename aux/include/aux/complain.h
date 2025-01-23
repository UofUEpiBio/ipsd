#ifndef IPSD_AUX_COMPLAIN_H
#define IPSD_AUX_COMPLAIN_H

/* This shouldn't be used outside of horrible, horrible scenarios. */
#define ipsd_fatal(title, fmt, ...)                                                                \
    ipsd_fatal_impl(__FILE__, __func__, __LINE__, title, fmt, ##__VA_ARGS__)

#define ipsd_error(fmt, ...) ipsd_error_impl(__FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

#define ipsd_warning(fmt, ...) ipsd_warning_impl(__FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

#define ipsd_info(fmt, ...) ipsd_info_impl(__FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

/* This shouldn't be used outside of horrible, horrible scenarios. */
void ipsd_fatal_impl(
  const char *file,
  const char *func,
  const int line,
  const char *title,
  const char *fmt,
  ...
);
void ipsd_error_impl(const char *file, const char *func, const int line, const char *fmt, ...);
void ipsd_warning_impl(const char *file, const char *func, const int line, const char *fmt, ...);
void ipsd_info_impl(const char *file, const char *func, const int line, const char *fmt, ...);

#endif /* IPSD_AUX_COMPLAIN_H */
