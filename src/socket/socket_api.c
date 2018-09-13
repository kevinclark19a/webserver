#include "socket/socket_api.h"

#include "logging/log_api.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>

#include <netinet/in.h>


static struct sockaddr_in __address;

//////////////////
//	API	//
//////////////////

int safely_initialize_socket(unsigned int port) {
	// Note that this function does not need to be thread
	// safe because it is only called from the main thread.
	// More importantly, it is only called *before* the
	// thread pool is even up and running.
	// If that ever changes, this function needs to be made
	// thread safe!
	int server_fd;

	if ( (server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0 ) {
		Log_e("Error opening server socket. Aborting.");
		exit(-1);
	}

	__address.sin_family = AF_INET;
	__address.sin_addr.s_addr = INADDR_ANY;
	__address.sin_port = htons(port);

	memset(__address.sin_zero, '\0', sizeof(__address.sin_zero));

	const int on = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	if (bind(server_fd, (struct sockaddr *)&__address, sizeof(__address)) < 0) {
		Log_e("Error binding socket to file descriptor. Aborting.");
		exit(-1);
	}

	if (listen(server_fd, SOMAXCONN) < 0) {
		Log_e("Error Listening to sockect. Aborting.");
		exit(-1);
	}

	return server_fd;
}

int accept_viable_connection(int server_fd) {
	int new_socket;

	int addrlen = sizeof(__address);

	while ( (new_socket = accept(server_fd, (struct sockaddr *)&__address, (socklen_t *)&addrlen)) < 0 )
		Log_w("Non-viable connection heard on bound socket. Skipping.");

	return new_socket;
}
