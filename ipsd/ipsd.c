#include "articulate.h"
#include "seatbelt.h"
#include "dispatch.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <aux/complain.h>
#include <aux/entry.h>
#include <aux/memory/libc.h>
#include <aux/memory/region.h>
#include <aux/memory/memory.h>

#include <microhttpd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

static struct MHD_Daemon *daemon_oth;
static bool running = true;

static void handle_signal(int sig) {
    switch (sig) {
    case SIGINT:
    case SIGTERM:
        ipsd_info("qualifying signal caught, slaying the daemon.");
        goto slay;

    default:
        /* Who cares? */
        return;
    }

slay:
    running = false;
}

static enum MHD_Result answer_to_connection(
  void *cls,
  struct MHD_Connection *connection,
  const char *url,
  const char *method,
  const char *version,
  const char *upload_data,
  size_t *upload_data_size,
  void **con_cls
) {
    struct IPSD_Articulation articulation;
    struct IPSD_Allocator libc_allocator;
    struct IPSD_Allocator region_allocator;
    struct IPSD_LibcData libc_data;
    struct IPSD_RegionData region_data;

    ipsd_init_libc_allocator(&libc_allocator, &libc_data);
    ipsd_init_region_allocator(&region_allocator, &libc_allocator, &region_data);

    /* We don't have any endpoints besides the one, and we only accept post. */
    if (strcmp(method, "POST") != 0) {
        ipsd_articulation_from_status_code(&articulation, MHD_HTTP_METHOD_NOT_ALLOWED);
        return ipsd_send_articulation(connection, &articulation);
    }

    if (strcmp(url, "/execute/python") != 0) {
        ipsd_articulation_from_status_code(&articulation, MHD_HTTP_NOT_FOUND);
        return ipsd_send_articulation(connection, &articulation);
    }

    /* Initialize connection-specific data. */
    char **post_data = (char **)con_cls;
    if (*post_data == NULL) {
        /* Allocate memory for accumulating POST data. */
        /* Start with an empty string. */
        *post_data = ipsd_alloc(&region_allocator, 1);
        (*post_data)[0] = 0;

        if (*post_data == NULL) {
            ipsd_articulation_from_status_code(&articulation, MHD_HTTP_INTERNAL_SERVER_ERROR);
            return ipsd_send_articulation(connection, &articulation);
        }

        return MHD_YES;
    }

    /* Accumulate POST data */
    if (*upload_data_size > 0) {
        size_t current_length = strlen(*post_data);

        char *new_data
          = ipsd_realloc(&region_allocator, *post_data, current_length + *upload_data_size + 1);
        if (new_data == NULL) {
            *post_data = NULL;
            ipsd_articulation_from_status_code(&articulation, MHD_HTTP_INTERNAL_SERVER_ERROR);
            return ipsd_send_articulation(connection, &articulation);
        }

        *post_data = new_data;
        memcpy(*post_data + current_length, upload_data, *upload_data_size);
        (*post_data)[current_length + *upload_data_size] = '\0';
        *upload_data_size = 0;

        return MHD_YES;
    }

    /* Sanity. */
    if (*post_data == NULL) {
        ipsd_articulation_from_status_code(&articulation, MHD_HTTP_BAD_REQUEST);
        return ipsd_send_articulation(connection, &articulation);
    }

    ipsd_info("[%s : %s@%s]", method, version, url);
    struct MHD_Response *response
      = ipsd_dispatch_interpreter_process(*post_data, IPSD_LANGUAGE_PYTHON);

    /* Return our results. */
    articulation = (struct IPSD_Articulation) {
      .code = 200,
      .type = IPSD_ARTICULATION_TEXT,
      .response = response,
    };

    int send_result = ipsd_send_articulation(connection, &articulation);

    /* But first, cleanup. */
    ipsd_free_region(&region_allocator);

    return send_result;
}

int main(int argc, char *argv[]) {
    ipsd_init_tool(argc, argv);
    ipsd_seatbelt_issue_relavent_warnings();

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    daemon_oth = MHD_start_daemon(
      MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
      IPSD_PORT,
      NULL,
      NULL,
      &answer_to_connection,
      NULL,
      MHD_OPTION_END
    );

    if (NULL == daemon_oth) {
        return EXIT_FAILURE;
    }

    /* Daemon runs in the background, so we just fake it being in the forefront of our minds. */
    ipsd_info("up, send signal to terminate.");
    while (running) {
        sleep(1);
    }

    MHD_stop_daemon(daemon_oth);

    return EXIT_SUCCESS;
}
