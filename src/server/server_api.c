#include "server/server_api.h"

#include "http/http_interface.h"
#include "http/rest_api.h"
#include "logging/log_api.h"
#include "server/rest_methods.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/**
 * Initializes the rest api.
 */
static void __initialize_REST_bindings(void);


static pthread_mutex_t __mutex;

static bool __initialized = false;

//////////////////
//	API	//
//////////////////

void handle_new_connection(void *new_socket) {
	int fd = *(int *)new_socket;

	if (!__initialized) {
		pthread_mutex_lock(&__mutex);
		__initialize_REST_bindings();
		pthread_mutex_unlock(&__mutex);
	}

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

static void __initialize_REST_bindings() {
	if (__initialized) return;

	add_GET_binding("/", get_index);

	__initialized = true;
}
