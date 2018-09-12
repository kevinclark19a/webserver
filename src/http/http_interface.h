#ifndef HTTP_INT_H
#define HTTP_INT_H

enum _http_verb {
	HTTP_VERB_GET,
	HTTP_VERB_PUT,
	HTTP_VERB_POST,
	HTTP_VERB_DELETE
};

struct _http_request {
	unsigned int version;

	enum _http_verb action;
	char *path;
};

struct _http_response {
	unsigned int status_code;
	char *short_message;

	char *response_type;

	unsigned int num_bytes;
	char *body;
};

typedef struct _http_request http_request_t;
typedef struct _http_response http_response_t;

/**
 * Reads from the specified socket and parses
 * an http request if one exists. Otherwise,
 * returns NULL.
 */
http_request_t *get_http_request(int socket);

/**
 * Writes the specified response to the given socket.
 */
void write_http_response(int socket, http_response_t *res);

#endif	// HTTP_INT_H
