#include "seatbelt.h"
#include <aux/complain.h>

void ipsd_seatbelt_issue_relavent_warnings() {
#if defined(IPSD_SEATBELT_IS_PEDAL_TO_THE_METAL)
    ipsd_warning("Seatbelt doesn't exist, malicious code execution is easy.");
#elif defined(IPSD_SEATBELT_IS_WEAK)
    ipsd_warning(
      "seatbelt is using method '%s', malicious code execution is easy.", IPSD_SEATBELT_METHOD_HUMAN
    );
#else
    ipsd_info("using a strong seatbelt (method '%s').", IPSD_SEATBELT_METHOD_HUMAN);
    ipsd_info("malicious code execution is now considered a bug.");
#endif
}

#if defined(IPSD_SEATBELT_USE_NONE)
int ipsd_seatbelt_bucket_up() {
    return 0;
}
#elif defined(IPSD_SEATBELT_USE_SANDBOX_INIT)
#include <sandbox.h>

#define RESTRICT_SANDBOX_PROFILE                                                                   \
    "(version 1)\n"                                                                                \
    ""                                                                                             \
    ";; Default deny all actions.\n"                                                               \
    "(deny default)\n"                                                                             \
    ""                                                                                             \
    ";; Basic operations.\n"                                                                       \
    "(allow sysctl-read)\n"                                                                        \
    ""                                                                                             \
    ";; Allow modules.\n"                                                                          \
    "(allow file-read* (regex #\"(?i).*\\.py$\"))\n"                                               \
    "(allow file-read* (regex #\"(?i).*\\.matplotlib$\"))\n"                                       \
    "(allow file-write* (regex #\"*\\.matplotlib*\"))"                                             \
    "(allow file-read* (regex #\"(?i).*site-packages.*\"))\n"                                      \
    "(allow file-read* (regex #\"(?i).*python.*\"))\n"                                             \
    ""                                                                                             \
    ";; Allow messages.\n"                                                                         \
    "(allow file-write* (regex #\"^/dev/tty\"))\n"

#define ALLOW_SANDBOX_PROFILE                                                                      \
    "(version 1)\n"                                                                                \
    "(allow default)"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
int ipsd_seatbelt_buckle_up() {
    char *error;
    int res;

    res = sandbox_init(RESTRICT_SANDBOX_PROFILE, 0, &error);

    if (res != 0) {
        ipsd_error("sandbox (seatbelt) initiation failed: %s", error);
        sandbox_free_error(error);

        return 1;
    }

    return 0;
}
#pragma clang diagnostic pop
#elif defined(IPSD_SEATBELT_USE_SECCOMP)
#error seccomp(2) support is unimplemented.
#elif defined(IPSD_SEATBELT_USE_PLEDGE)
#error pledge(2) support is unimplemented.
#endif
