#include "http/rest_api.h"

#include "hashmap/hash_map.h"
#include "http/http_interface.h"
#include "logging/log_api.h"

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * The default 404 not found display.
 */
static http_response_t *__get_not_found(void);

//////////////////
//	API	//
//////////////////

static hash_map_t path_mapping;

void add_REST_binding(const char *const path, http_response_t *(*gen_func)(void)) {
	add_mapping(&path_mapping, path, (void *)gen_func);
}

http_response_t *envoke_REST_binding(http_request_t *req) {
	if (!has_mapping(&path_mapping, req->path)) return __get_not_found();

	http_response_t *(*gen_func)(void) = ( http_response_t *(*)(void) )
		get_mapping(&path_mapping, req->path);
	Log_d("Getting Here.");
	return gen_func();
}

//////////////////////////////////
//	HELPER FUNCTIONS	//
//////////////////////////////////

static http_response_t *__get_not_found() {
	http_response_t *res = (http_response_t *)calloc(1, sizeof(http_response_t));

	res->status_code = 404;
	res->short_message = "NOT FOUND";
	res->response_type = "text/html";
	res->body =	"<html>"
			  "<h2>Page not found.</h2>"
			  "<p>That's an error- 404, to be precise.</p>"
			"</html>";

	return res;
}
