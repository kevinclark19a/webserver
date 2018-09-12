#ifndef REST_H
#define REST_H

#include "http/http_interface.h"

/**
 * Binds the function to the specified path. When envoke_REST_binding
 * is called with the same path specified, this function will be
 * called.
 */
void add_REST_binding(const char *const path, http_response_t * (*gen_func)(void));

/**
 * Calls the function bound to the specified path to build
 * the resultant http_response object.
 */
http_response_t *envoke_REST_binding(http_request_t *req);

#endif	// REST_H
