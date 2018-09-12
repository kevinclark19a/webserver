#include "http/http_interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Creates a correctly formatted response header from the
 * specified response object.
 */
static const char *const __create_response_header(http_response_t *res);

//////////////////
//	API	//
//////////////////

http_request_t *get_http_request(int socket) {
	http_request_t *req = (http_request_t *)calloc(1, sizeof(http_request_t));

	req->version = 11;
	req->path = "/";
	req->action = HTTP_VERB_GET;

	return req;
}

void write_http_response(int socket, http_response_t *res) {
	char *str = (char *)calloc(4096, sizeof(char));
	const char *response_header;
	sprintf(str, "%s\n%s\n",
		(response_header = __create_response_header(res)),
		res->body);
	free( (void *)response_header );

	write(socket, str, strlen(str));
}

//////////////////////////////////
//	HELPER FUNCTIONS	//
//////////////////////////////////

static const char *const __create_response_header(http_response_t *res) {
	char *str = (char *)calloc(256, sizeof(char));
	sprintf(str,
		"HTTP/1.1 %u %s\n"
		"Server: " WEBSERVER__PROGRAM_NAME "\n"
		"Content-Type: %s\n"
		"Content-Length: %lu\n"
		"Connection: Closed\n",
		res->status_code,
		res->short_message,
		res->response_type,
		strlen(res->body));

	return str;
}
