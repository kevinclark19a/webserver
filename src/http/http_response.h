#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

struct _http_response {
	unsigned int status_code;
	char *short_message;

	unsigned int num_bytes;
	char *response_type;
	char *body;
};

typedef struct _http_response http_response_t;

static const http_response_t http_response_ok200  = {
	.status_code = 200,
	.short_message = "OK"
};

static const http_response_t http_response_error404 = {
	.status_code = 404,
	.short_message = "NOT FOUND",

	.response_type = "text/html",
};

/**
 * Clones the specified response into a new memory location.
 */
http_response_t *clone_http_response(const http_response_t *original);

#endif	// HTTP_RESPONSE_H
