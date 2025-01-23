#ifndef IPSD_AUX_DEVEL_H
#define IPSD_AUX_DEVEL_H

#ifdef IPSD_DEV
#define IPSD_DEVELOPMENT_ONLY

#define ipsd_assert(cond)                                                                          \
    do {                                                                                           \
        if (!(cond)) {                                                                             \
            ipsd_fatal("false assert", "assert `" #cond "' ended up resolving to false");          \
            abort();                                                                               \
        }                                                                                          \
    } while (0)
#else
#define IPSD_DEVELOPMENT_ONLY                                                                      \
    ipsd_warning(                                                                                  \
      "development only function `%s' is being called in a "                                       \
      "non-development context.",                                                                  \
      __func__                                                                                     \
    )

#define ipsd_assert(cond)
#endif /* IPSD_DEV */

#define IPSD_UNIMPLEMENTED                                                                         \
    do {                                                                                           \
        ipsd_fatal("unimplemented", "encountered unimplemented code");                             \
        abort();                                                                                   \
    } while (0)

#define IPSD_SWITCH_EXHAUSTIVE                                                                     \
    do {                                                                                           \
        ipsd_fatal("unhandled case", "switch case isn't exhaustive");                              \
        abort();                                                                                   \
    } while (0)

#define IPSD_BAD_TYPE(msg)                                                                         \
    do {                                                                                           \
        ipsd_fatal("bad type", msg);                                                               \
        abort();                                                                                   \
    } while (0);

#define IPSD_OUT_OF_RANGE(i, length)                                                               \
    do {                                                                                           \
        ipsd_fatal("out of range", "index %d out of range, max is %d", i, length);                 \
        abort();                                                                                   \
    } while (0);

#endif /* IPSD_AUX_DEVEL_H */
