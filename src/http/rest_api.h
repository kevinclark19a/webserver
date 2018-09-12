#ifndef REST_H
#define REST_H

#include "http/http_interface.h"

/**
 * Binds the function to the specified path. When envoke__binding
 * is called with the same path specified, this function will be
 * called.
 */
void add_GET_binding(const char *const path, http_response_t * (*response_func)(http_request_t *));

/**
 * Binds the function to the specified path. When envoke__binding
 * is called with the same path specified, this function will be
 * called.
 */
void add_PUT_binding(const char *const path, http_response_t * (*response_func)(http_request_t *));

/**
 * Binds the function to the specified path. When envoke__binding
 * is called with the same path specified, this function will be
 * called.
 */
void add_POST_binding(const char *const path, http_response_t * (*response_func)(http_request_t *));

/**
 * Binds the function to the specified path. When envoke__binding
 * is called with the same path specified, this function will be
 * called.
 */
void add_DELETE_binding(const char *const path, http_response_t * (*response_func)(http_request_t *));

/**
 * Calls the function bound to the specified path to build
 * the resultant http_response object.
 */
http_response_t *envoke__binding(http_request_t *req);

#endif	// REST_H
