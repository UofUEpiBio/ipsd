#ifndef IPSD_ARTICULATE_H
#define IPSD_ARTICULATE_H

#include <microhttpd.h>

extern char *ipsd_articlation_mime_types[];

enum IPSD_ArticulationType { IPSD_ARTICULATION_TEXT, IPSD_ARTICULATION_JSON };

struct IPSD_Articulation {
    enum IPSD_ArticulationType type;
    struct MHD_Response *response;
    int code;
};

void ipsd_articulation_from_status_code(struct IPSD_Articulation *articulation, int status_code);
void ipsd_articulation_with_custom_message(
  struct IPSD_Articulation *articulation,
  int status_code,
  const char *custom_message
);
void ipsd_articulation_from_errno(struct IPSD_Articulation *articulation, const char *context);
void ipsd_articulation_raw(
  struct IPSD_Articulation *articulation,
  int code,
  enum IPSD_ArticulationType type,
  const char *content
);

void ipsd_articulation_destroy(struct IPSD_Articulation *articulation);

enum MHD_Result
ipsd_send_articulation(struct MHD_Connection *connection, struct IPSD_Articulation *articulation);

#endif /* IPSD_ARTICULATE_H */
