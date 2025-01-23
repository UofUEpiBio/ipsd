#include "articulate.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

/* Parallel with IPSD_ArticulationType. */
char *ipsd_articlation_mime_types[] = {"text/plain", "application/json"};

static const char *get_default_message(int status_code) {
    switch (status_code) {
    case MHD_HTTP_OK:
        return "200 OK";
    case MHD_HTTP_BAD_REQUEST:
        return "400 Bad Request";
    case MHD_HTTP_NOT_FOUND:
        return "404 Not Found";
    case MHD_HTTP_INTERNAL_SERVER_ERROR:
        return "500 Internal Server Error";
    case MHD_HTTP_METHOD_NOT_ALLOWED:
        return "405 Method Not Allowed";
    default:
        return "No entry in message humanization table, check MDN.";
    }
}

void ipsd_articulation_from_status_code(struct IPSD_Articulation *articulation, int status_code) {
    const char *default_message = get_default_message(status_code);
    char error_message[1024];

    snprintf(
      error_message,
      sizeof(error_message),
      "{\"success\": %s, \"code\": %d, \"human\": \"%s\"}",
      status_code == 200 ? "true" : "false",
      status_code,
      default_message
    );

    articulation->type = IPSD_ARTICULATION_JSON;
    articulation->code = status_code;
    articulation->response = MHD_create_response_from_buffer(
      strlen(error_message), (void *)error_message, MHD_RESPMEM_PERSISTENT
    );
}

void ipsd_articulation_with_custom_message(
  struct IPSD_Articulation *articulation,
  int status_code,
  const char *custom_message
) {
    char error_message[1024];

    snprintf(
      error_message,
      sizeof(error_message),
      "{\"success\": %s, \"code\": %d, \"human\": \"%s\"}",
      status_code == 200 ? "true" : "false",
      status_code,
      custom_message
    );

    articulation->type = IPSD_ARTICULATION_JSON;
    articulation->code = status_code;
    articulation->response = MHD_create_response_from_buffer(
      strlen(error_message), (void *)error_message, MHD_RESPMEM_PERSISTENT
    );
}

void ipsd_articulation_from_errno(struct IPSD_Articulation *articulation, const char *context) {
    char error_message[1024];

    snprintf(
      error_message,
      sizeof(error_message),
      "{\"success\": %s, \"code\": %d, \"human\": \"%s: %s\"}",
      errno == 0 ? "true" : "false",
      errno,
      context,
      strerror(errno)
    );

    articulation->type = IPSD_ARTICULATION_JSON;
    articulation->code = errno;
    articulation->response = MHD_create_response_from_buffer(
      strlen(error_message), (void *)error_message, MHD_RESPMEM_PERSISTENT
    );
}

void ipsd_articulation_raw(
  struct IPSD_Articulation *articulation,
  int code,
  enum IPSD_ArticulationType type,
  const char *content
) {
    articulation->code = code;
    articulation->type = type;
    articulation->response
      = MHD_create_response_from_buffer(strlen(content), (void *)content, MHD_RESPMEM_PERSISTENT);
}

void ipsd_articulation_destroy(struct IPSD_Articulation *articulation) {
    MHD_destroy_response(articulation->response);
}

enum MHD_Result
ipsd_send_articulation(struct MHD_Connection *connection, struct IPSD_Articulation *articulation) {
    enum MHD_Result ret;
    const char *mime_type = ipsd_articlation_mime_types[articulation->type];

    MHD_add_response_header(articulation->response, "Content-Type", mime_type);
    ret = MHD_queue_response(connection, articulation->code, articulation->response);

    ipsd_articulation_destroy(articulation);

    return ret;
}
