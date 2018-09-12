#include "http/rest_api.h"

#include "hashmap/hash_map.h"
#include "http/http_interface.h"
#include "logging/log_api.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/**
 * The default 404 not found display.
 */
static http_response_t *__get_not_found(void);

static hash_map_t get_map;

static hash_map_t put_map;

static hash_map_t post_map;

static hash_map_t delete_map;

//////////////////
//	API	//
//////////////////

void add_GET_binding(const char *const path, http_response_t *(*response_func)(http_request_t *)) {
	add_mapping(&get_map, path, (void *)response_func);
}

void add_PUT_binding(const char *const path, http_response_t *(*response_func)(http_request_t *)) {
	add_mapping(&put_map, path, (void *)response_func);
}

void add_POST_binding(const char *const path, http_response_t *(*response_func)(http_request_t *)) {
	add_mapping(&post_map, path, (void *)response_func);
}

void add_DELETE_binding(const char *const path, http_response_t *(*response_func)(http_request_t *)) {
	add_mapping(&delete_map, path, (void *)response_func);
}

http_response_t *envoke__binding(http_request_t *req) {

	http_response_t *(*response_func)(http_request_t *);

	switch (req->action) {

	case HTTP_VERB_GET:
		if (!has_mapping(&get_map, req->path)) return __get_not_found();

		response_func = ( http_response_t *(*)(http_request_t *) )
			get_mapping(&get_map, req->path);

		return response_func(req);

	case HTTP_VERB_PUT:
		if (!has_mapping(&put_map, req->path)) return __get_not_found();

		response_func = ( http_response_t *(*)(http_request_t *) )
			get_mapping(&put_map, req->path);

		return response_func(req);

	case HTTP_VERB_POST:
		if (!has_mapping(&post_map, req->path)) return __get_not_found();

		response_func = ( http_response_t *(*)(http_request_t *) )
			get_mapping(&post_map, req->path);

		return response_func(req);

	case HTTP_VERB_DELETE:
		if (!has_mapping(&delete_map, req->path)) return __get_not_found();

		response_func = ( http_response_t *(*)(http_request_t *) )
			get_mapping(&delete_map, req->path);

		return response_func(req);
	}

	return __get_not_found();
}

//////////////////////////////////
//	HELPER FUNCTIONS	//
//////////////////////////////////

static http_response_t *__get_not_found() {
	http_response_t *res = (http_response_t *)calloc(1, sizeof(http_response_t));

	Log_d("Getting to __get_not_found");

	res->status_code = 404;
	res->short_message = "NOT FOUND";
	res->response_type = "text/html";

	res->body =	"<html>"
			  "<h2>Page not found.</h2>"
			  "<p>That's an error- 404, to be precise.</p>"
			"</html>";

	res->num_bytes = strlen(res->body);

	return res;
}
