#include "dispatch.h"
#include "language.h"
#include "seatbelt.h"
#include "support/python.h"
#include <aux/complain.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <microhttpd.h>

struct MHD_Response *ipsd_dispatch_interpreter_process(char *source, enum IPSD_Language language) {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        ipsd_error("failed to open pipe for interpreter wrapper communication: %s", strerror(errno));
        return NULL;
    }

    pid = fork();
    if (pid == -1) {
        ipsd_error("failed to fork interpreter wrapper: %s", strerror(errno));
        return NULL;
    }

    if (pid == 0) {
        /* CHILD */
        close(pipefd[0]); /* Close the read end. */

        /* Strap on our seatbelt. */
        ipsd_seatbelt_buckle_up();

        /* Do some logic. */
        switch (language) {
        case IPSD_LANGUAGE_PYTHON:
            ipsd_support_python_run_script(source, pipefd);
            break;
        case IPSD_LANGUAGE_R:
            abort(/* I'm too lazy to handle this case. */);
        }

        close(pipefd[1]); /* Close the write end. */
        exit(EXIT_SUCCESS);
    } else {
        /* PARENT */
        close(pipefd[1]); /* Close the write end. */

        struct MHD_Response *response = MHD_create_response_from_pipe(pipefd[0]);

        wait(NULL);
        return response;
    }
}
