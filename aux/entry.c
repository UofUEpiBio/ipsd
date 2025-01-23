#include <aux/entry.h>

#include <stdlib.h>

const char *ipsd_get_progname() {
    return ipsd_progname(NULL);
}

const char *ipsd_progname(const char *name) {
    static const char *progname;

    if (name != NULL) {
        return progname = name;
    } else {
        return progname;
    }
}

void ipsd_init_tool(int argc, char *argv[]) {
    ipsd_progname(argv[0]);
}
