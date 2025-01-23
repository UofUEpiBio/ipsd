#ifndef IPSD_SEATBELT_H
#define IPSD_SEATBELT_H

#include <aux/platform.h>

#if defined(IPSD_PLAT_APPLE)
#define IPSD_SEATBELT_IS_STRONG
#define IPSD_SEATBELT_USE_SANDBOX_INIT
#define IPSD_SEATBELT_METHOD_HUMAN "sandbox_init"
#elif defined(IPSD_PLAT_MACH)
#define IPSD_SEATBELT_IS_PEDAL_TO_THE_METAL
#define IPSD_SEATBELT_USE_NONE
#define IPSD_SEATBELT_METHOD_HUMAN "none"
#elif defined(IPSD_PLAT_LINUX)
#define IPSD_SEATBELT_IS_STRONG
#define IPSD_SEATBELT_USE_SECCOMP
#define IPSD_SEATBELT_METHOD_HUMAN "seccomp"
#elif defined(IPSD_PLAT_OPEN_BSD)
#define IPSD_SEATBELT_IS_STRONG
#define IPSD_SEATBELT_USE_PLEDGE
#define IPSD_SEATBELT_METHOD_HUMAN "pledge"
#elif defined(IPSD_PLAT_LOSEDOWS)
#define IPSD_SEATBELT_IS_PEDAL_TO_THE_METAL
#define IPSD_SEATBELT_USE_NONE
#define IPSD_SEATBELT_METHOD_HUMAN "none"
#else
#define IPSD_SEATBELT_IS_PEDAL_TO_THE_METAL
#define IPSD_SEATBELT_USE_NONE
#define IPSD_SEATBELT_METHOD_HUMAN "none"
#endif /* Seatbelt support matrix. */

void ipsd_seatbelt_issue_relavent_warnings();
int ipsd_seatbelt_buckle_up();

#endif /* IPSD_SEATBELT_H */
