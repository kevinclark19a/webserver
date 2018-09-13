#ifndef REST_METHODS_H
#define REST_METHODS_H

#include "http/http_interface.h"

/**
 * Returns the index file.
 */
http_response_t *REST_method__get_index(http_request_t *req);

#endif	// REST_METHODS_H
