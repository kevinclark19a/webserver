#include "server/server_api.h"

#include "hashmap/hash_map.h"
#include "http/http_interface.h"
#include "http/http_response.h"
#include "logging/log_api.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Helper function to call the correct function based
 * on the passed request.
 */
static http_response_t *envoke__binding(http_request_t *req);


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

void handle_new_connection(void *new_socket) {
	int fd = *(int *)new_socket;

	free(new_socket);

	http_request_t *req = get_http_request(fd);
	if (req != NULL) {
		http_response_t *res = envoke__binding(req);
		write_http_response(fd, res);

		free( (void *)req );

		char *str = (char *)calloc(1024, sizeof(char));
		sprintf(str, "Served a request for path '%s'.", req->path);
		Log_i( (const char *const)str );

		free( (void *)str );
	}

	close(fd);
}

//////////////////////////////////
//	HELPER FUNCTIONS	//
//////////////////////////////////

static http_response_t *envoke__binding(http_request_t *req) {

	http_response_t *(*response_func)(http_request_t *);

	switch (req->action) {

	case HTTP_VERB_GET:
		if (!has_mapping(&get_map, req->path))
			return clone_http_response(&http_response_error404);

		response_func = ( http_response_t *(*)(http_request_t *) )
			get_mapping(&get_map, req->path);

		return response_func(req);

	case HTTP_VERB_PUT:
		if (!has_mapping(&put_map, req->path))
			return clone_http_response(&http_response_error404);

		response_func = ( http_response_t *(*)(http_request_t *) )
			get_mapping(&put_map, req->path);

		return response_func(req);

	case HTTP_VERB_POST:
		if (!has_mapping(&post_map, req->path))
			return clone_http_response(&http_response_error404);

		response_func = ( http_response_t *(*)(http_request_t *) )
			get_mapping(&post_map, req->path);

		return response_func(req);

	case HTTP_VERB_DELETE:
		if (!has_mapping(&delete_map, req->path))
			return clone_http_response(&http_response_error404);

		response_func = ( http_response_t *(*)(http_request_t *) )
			get_mapping(&delete_map, req->path);

		return response_func(req);
	}

	return clone_http_response(&http_response_error404);
}
