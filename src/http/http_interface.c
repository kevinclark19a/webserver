#include "http/http_interface.h"

#include "logging/log_api.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Creates a correctly formatted response header from the
 * specified response object.
 */
static const char *const __create_response_header(http_response_t *res);

/**
 * Parses the passed query and extracts header information. Stores the
 * resultant info in req and returns true if successful and false otherwise.
 * queryp will have the header popped off if there was one, and will be
 * untouched otherwise.
 */
static bool __parse_http_request_header(char *queryp[], http_request_t *req);

//////////////////
//	API	//
//////////////////

http_request_t *get_http_request(int socket) {
	http_request_t *req = (http_request_t *)calloc(1, sizeof(http_request_t));

	char *buffer = (char *)calloc(4096, sizeof(char));
	read(socket, buffer, 4095);

	Log_d( (const char *const)buffer );

	char *query = buffer;
	if (!__parse_http_request_header(&query, req)) {
		free( (void *)req );
		free( (void *)buffer );
		return NULL;
	}

	// TODO: Implement more complicated parsing for things
	// other than the header.

	free( (void *) buffer);

	return req;
}

void write_http_response(int socket, http_response_t *res) {

	const char *response_header = __create_response_header(res);
	int header_len = strlen(response_header);

	char *str = (char *)calloc(header_len + res->num_bytes, sizeof(char));
	strcpy(str, response_header);
	free( (void *)response_header );

	memcpy(str + header_len, res->body, res->num_bytes);

	write(socket, str, header_len + res->num_bytes);
	free( (void *)str );
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
		"Content-Length: %u\n"
		"Connection: Closed\n\n",
		res->status_code, res->short_message,
		res->response_type, res->num_bytes);

	return str;
}

static bool __parse_http_request_header(char *queryp[], http_request_t *req) {

	// Pop and store the verb
	if (strstr(*queryp, "GET ") == *queryp) {
		req->action = HTTP_VERB_GET;
		*queryp += 4;
	} else if (strstr(*queryp, "PUT ") == *queryp) {
		req->action = HTTP_VERB_PUT;
		*queryp += 4;
	} else if (strstr(*queryp, "POST ") == *queryp) {
		req->action = HTTP_VERB_POST;
		*queryp += 5;
	} else if (strstr(*queryp, "DELETE ") == *queryp) {
		req->action = HTTP_VERB_DELETE;
		*queryp += 7;
	} else return false;

	// Pop and store the path
	char *next_space = strstr(*queryp, " ");

	req->path = (char *)calloc( (next_space - *queryp) + 1, sizeof(char));
	strncpy(req->path, *queryp, (next_space - *queryp));

	*queryp = next_space + 1;

	// Pop off the HTTP name from the header
	if (strstr(*queryp, "HTTP/") ==  NULL) return false;
	*queryp += 5;

	// Pop and store the version
	if (strstr(*queryp, "1.1") == *queryp) {
		req->version = 11;
		*queryp += 3;
	} else if (strstr(*queryp, "2") == *queryp) {
		req->version = 20;
		*queryp += 1;
	} else return false;

	// Pop the remaining newline
	if (strstr(*queryp, "\n") == *queryp) *queryp += 1;

	// Successfully parsed!
	return true;
}
