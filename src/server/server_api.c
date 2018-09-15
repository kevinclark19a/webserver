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

#define __BUFF_SIZE 4096

/**
 * Helper function to call the correct function based
 * on the passed request.
 */
static http_response_t *envoke__binding(http_request_t *req);

/**
 * Parses the specified request to figure out what
 * the response type should be by looking at the file
 * ending of the path.
 */
static char *__get_response_type(http_request_t *req);

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

	char *str = (char *)calloc(512, sizeof(char));

	http_request_t *req = get_http_request(fd);
	if (req != NULL) {
		http_response_t *res = envoke__binding(req);
		write_http_response(fd, res);

		free( (void *)res->body );
		free( (void *)res );

		sprintf(str, "Served a request for path '%s'.", req->path);
		Log_i( (const char *const)str );

		free( (void *)req->path );
		free( (void *)req );

	}

	free( (void *)str );

	close(fd);
}

http_response_t *serve_file(http_request_t *req) {
	char *filename = (char *)calloc(sizeof(WEBSERVER__WORKING_DIRECTORY "/res") + strlen(req->path), sizeof(char));
	sprintf(filename, "%s%s", WEBSERVER__WORKING_DIRECTORY "/res", req->path);

	if (strstr(filename, "./") != NULL) {
		free( (void *)filename );
		return clone_http_response(&http_response_error404);
	}

	FILE *fp = fopen(filename, "r");
	free( (void *)filename );

	if (fp == NULL) return clone_http_response(&http_response_error404);

	char *buffer = (char *)calloc(__BUFF_SIZE, sizeof(char));
	char *curr = buffer;

	int tot_bytes_read = 0;
	int bytes_read = 0;
	while ( (bytes_read = fread(curr, sizeof(char), __BUFF_SIZE, fp)) == __BUFF_SIZE ) {
		buffer = (char *)realloc(buffer, (tot_bytes_read += bytes_read) + __BUFF_SIZE );
		curr = buffer + tot_bytes_read;
	}
	tot_bytes_read += bytes_read;

	fclose(fp);

	http_response_t *res = clone_http_response(&http_response_ok200);

	res->response_type = __get_response_type(req);
	res->num_bytes = tot_bytes_read;
	res->body = buffer;

	return res;
}

//////////////////////////////////
//	HELPER FUNCTIONS	//
//////////////////////////////////

static http_response_t *envoke__binding(http_request_t *req) {

	http_response_t *(*response_func)(http_request_t *);

	switch (req->action) {

	case HTTP_VERB_GET:
		if (!has_mapping(&get_map, req->path))
			return serve_file(req);

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

static char *__get_response_type(http_request_t *req) {
	char *file_ending = req->path;

	do {
		file_ending = strstr(file_ending, ".");

		if (strstr(file_ending, ".") == NULL) break;
	} while (strstr(file_ending, ".") != file_ending);

	// Handle text...
	if (strcmp(file_ending, ".txt") == 0)
		return "text/plain";
	if (strcmp(file_ending, ".html") == 0)
		return "text/html";
	if (strcmp(file_ending, ".css") == 0)
		return "text/css";
	if (strcmp(file_ending, ".csv") == 0)
		return "text/csv";
	if (strcmp(file_ending, ".xml") == 0)
		return "text/xml";

	// Data...
	if (strcmp(file_ending, ".js") == 0)
		return "application/javascript";
	if (strcmp(file_ending, ".json") == 0)
		return "application/json";
	if (strcmp(file_ending, ".pdf") == 0)
		return "application/pdf";

	// Images...
	if (strcmp(file_ending, ".ico") == 0)
		return "image/x-icon";
	if (strcmp(file_ending, ".png") == 0)
		return "image/png";
	if (strcmp(file_ending, ".jpeg") == 0 || strcmp(file_ending, ".jpg") == 0)
		return "image/jpeg";
	if (strcmp(file_ending, ".gif") == 0)
		return "image/gif";

	// Audio...
	if (strcmp(file_ending, ".wav") == 0)
		return "audio/x-wav";

	// Video...
	if (strcmp(file_ending, ".mov") == 0)
		return "video/mov";
	if (strcmp(file_ending, ".mp4") == 0)
		return "video/mp4";

	return "text/plain";
}
