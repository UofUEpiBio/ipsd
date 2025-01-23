#ifndef IPSD_DISPATCH_H
#define IPSD_DISPATCH_H

#include "language.h"
#include <aux/memory/memory.h>
#include <microhttpd.h>

struct MHD_Response *ipsd_dispatch_interpreter_process(char *source, enum IPSD_Language language);

#endif /* IPSD_DISPATCH_H */
