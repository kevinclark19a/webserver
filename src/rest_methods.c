#include "rest_methods.h"

#include "http/http_interface.h"
#include "server/server_api.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//////////////////
//	API	//
//////////////////

http_response_t *REST_method__get_index(http_request_t *req) {
	free( (void *)req->path );
	req->path = (char *)calloc(sizeof("/index.html"), sizeof(char));

	sprintf(req->path, "%s", "/index.html");

	return serve_file(req);
}
