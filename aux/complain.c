#include <aux/complain.h>

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

extern const char *ipsd_get_progname();

void ipsd_fatal_impl(
  const char *file,
  const char *func,
  const int line,
  const char *title,
  const char *fmt,
  ...
) {
    va_list arglist;
    va_start(arglist, fmt);

    fprintf(stderr, "*** (");
    while (*title) {
        fprintf(stderr, "%c", toupper(*title));

        title++;
    }

    fprintf(stderr, ") %s", ipsd_get_progname());

    fprintf(stderr, "(%d,%p) %s:%s:%d:\n*** ", getpid(), fmt, file, func, line);
    vfprintf(stderr, fmt, arglist);
    fprintf(stderr, "\n");
    va_end(arglist);
}

void ipsd_error_impl(const char *file, const char *func, const int line, const char *fmt, ...) {
    va_list arglist;
    va_start(arglist, fmt);

#ifdef IPSD_DEV
    fprintf(stderr, "%s:%s:%d: ", file, func, line);
#endif /* IPSD_DEV */

    fprintf(stderr, "[server] [error] ");
    vfprintf(stderr, fmt, arglist);
    fprintf(stderr, "\n");
    va_end(arglist);
}

void ipsd_warning_impl(const char *file, const char *func, const int line, const char *fmt, ...) {
    va_list arglist;
    va_start(arglist, fmt);

#ifdef IPSD_DEV
    fprintf(stderr, "%s:%s:%d: ", file, func, line);
#endif /* IPSD_DEV */

    fprintf(stderr, "[server] [warning] ");
    vfprintf(stderr, fmt, arglist);
    fprintf(stderr, "\n");
    va_end(arglist);
}

void ipsd_info_impl(const char *file, const char *func, const int line, const char *fmt, ...) {
    va_list arglist;
    va_start(arglist, fmt);

#ifdef IPSD_DEV
    fprintf(stderr, "%s:%s:%d: ", file, func, line);
#endif /* IPSD_DEV */

    fprintf(stderr, "[server] ");
    vfprintf(stderr, fmt, arglist);
    fprintf(stderr, "\n");
    va_end(arglist);
}
