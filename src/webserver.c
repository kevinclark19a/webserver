
#include "logging/log_api.h"
#include "server/server_api.h"
#include "socket/socket_api.h"
#include "workqueue/workqueue_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static int PORT = 8080;

/**
 * Helper function to display usage.
 */
static int show_usage(void);

/**
 * Main entry point for the webserver.
 */
int main(int argc, char *const argv[]) {

	int c;

	while ( (c = getopt(argc, argv, "hp:")) > 0 ) {
		switch (c) {
		default :
		case 'h':
			return show_usage();
		case 'p':
			PORT = atoi(optarg);
			break;
		}
	}

	int server_fd = safely_initialize_socket(PORT);

	while (1) {

		Log_d("Service Idling, waiting on connection.");

		int *new_socket = (int*)calloc(1, sizeof(int));
		*new_socket = accept_viable_connection(server_fd);

		submit_task_to_pool(handle_new_connection,
			(void *)new_socket);

		Log_i("Connection accepted.");
	}
}

//////////////////////////////////
//	HELPER FUNCTIONS	//
//////////////////////////////////

static int show_usage(void) {
	printf(	"%s version %s\n"
		"\n"
		"usage: %s -[h|p] <portnum>\n"
		"\n"
		"The options provided are:\n"
		"\n"
			"\t-h : Shows this usage and exits.\n"
			"\t-p : Runs %s on the specified portnum.\n"
		"\n"
		"Note that if no options are passed a default value "
		"of 8080 is assigned for the portnum.",
		WEBSERVER__PROGRAM_NAME,
		WEBSERVER__VERSION,
		WEBSERVER__PROGRAM_NAME,
		WEBSERVER__PROGRAM_NAME);
	return 0;
}
