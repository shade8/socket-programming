/*
 * client.c
 *
 *  Created on: Dec 27, 2014
 *      Author: Blair
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main (int argc, char **argv) {
	enum {BUF_SIZE = 4096};

	int socket;
	char buf[BUF_SIZE];
	struct addrinfo hints;
	struct addrinfo *result;

	memset((void *) &hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(argv[1], argv[2], &hints, &result) != 0) {
		perror("Address info");
		exit(EXIT_FAILURE);
	}
	if ((socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if (connect(socket, result->ai_addr, result->ai_addrlen) == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(result);



}
