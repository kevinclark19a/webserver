#ifndef SOCKET_H
#define SOCKET_H

/**
 * Initializes and returns a socket tied to the passed
 * port.
 *
 * Exits if failure occurs.
 */
int safely_initialize_socket(unsigned int port);

/**
 * Listens on the specified socket until a viable connection
 * is heard, and then returns said connection. All non-viable
 * connections are ignored.
 */
int accept_viable_connection(int server_fd);

#endif	// SOCKET_H
