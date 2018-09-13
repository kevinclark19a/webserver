#include "http/http_response.h"

#include <stdlib.h>
#include <string.h>

//////////////////
//	API	//
//////////////////

http_response_t *clone_http_response(const http_response_t *original) {
	http_response_t *clone = (http_response_t *)calloc(1, sizeof(http_response_t));

	clone->status_code = original->status_code;
	clone->short_message = original->short_message;
	clone->response_type = original->response_type;

	if (original->body == NULL) return clone;

	clone->num_bytes = original->num_bytes;

	clone->body = (char *)calloc(clone->num_bytes, sizeof(char));

	memcpy(clone->body, original->body, clone->num_bytes);

	return clone;
}
