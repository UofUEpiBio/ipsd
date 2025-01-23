#ifndef IPSD_AUX_PLATFORM_H
#define IPSD_AUX_PLATFORM_H

#if defined(__MACH__) && defined(__APPLE__)
#define IPSD_PLAT_APPLE
#elif defined(__MACH__)
#define IPSD_PLAT_MACH
#elif defined(__linux__) || defined(__linux) || defined(linux)
#define IPSD_PLAT_LINUX
#elif defined(__OpenBSD__)
#define IPSD_PLAT_OPEN_BSD
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define IPSD_PLAT_LOSEDOWS /* heh, i'm 5. */
#endif

#endif /* IPSD_AUX_PLATFORM_H */
