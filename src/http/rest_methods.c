#include "http/rest_methods.h"

#include "http/http_interface.h"

#include <stdlib.h>

//////////////////
//	API	//
//////////////////

http_response_t *get_index() {
	http_response_t *res = (http_response_t *)calloc(1, sizeof(http_response_t));

	res->status_code = 200;
	res->short_message = "OK";
	res->response_type = "text/plain";
	res->body = "Hello, World.";

	return res;
}
