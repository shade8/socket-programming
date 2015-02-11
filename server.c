/*
 * 	server.c
 *
 *  Created on: Dec 26, 2014
 *      Author: Blair
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char **argv) {
	enum {MAX_BACKLOG = 5, BUF_SIZE = 4096};

   int port = strtol(argv[1], NULL, 0);
   int socket;
   int newSocket;
   size_t addrLen;
   size_t bytesRead;
   char buf[BUF_SIZE];
   struct sockaddr_in serverAddr, clientAddr;
   FILE *psFile;

   if (port == 0) {
      fprintf(stderr, "Invalid port number.");
      exit(EXIT_FAILURE);
   }
   if ((port == LONG_MAX) || (port == LONG_MIN)) {
	   perror("Port number");
	   exit(EXIT_FAILURE);
   }


   memset((void *) &serverAddr, 0, sizeof(serverAddr));
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(port);
   serverAddr.in_addr.s_addr = INADDR_ANY;

   if ((socket = socket(PF_INET, SOCK_STREAM, UNSPEC)) == -1) {
	   perror("socket");
	   exit(EXIT_FAILURE);
   }
   if (bind(socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
	   perror("bind");
	   exit(EXIT_FAILURE);
   }
   if (listen(socket, MAX_BACKLOG) == -1) {
	   perror("listen");
	   exit(EXIT_FAILURE);
   }

   addrLen = sizeof(clientAddr);
   for ( ; ; ) {
	   if ((newSocket = accept(socket, (struct sockaddr *) &clientAddr, &addrLen)) == -1) {
		   perror("accept");
		   exit(EXIT_FAILURE);
	   }
	   if ((psFile = fdopen(newSocket, "r")) == NULL) {
		   perror("input stream");
		   exit(EXIT_FAILURE);
	   }
	   while ((bytesRead = fread((void *) buf, 1, BUF_SIZE, psFile)) == BUF_SIZE)
		   if (fwrite((void *) buf, 1, bytesRead, stdout) != bytesRead) {
			   perror("write");
			   exit(EXIT_FAILURE);
		   }
	   if (ferror(psFile)) {
		   perror("read");
		   exit(EXIT_FAILURE);
	   }
	   if (fwrite((void *) buf, 1, bytesRead, stdout) != bytesRead) {
		   perror("write");
		   exit(EXIT_FAILURE);
	   }
	   if (close(newSocket) == -1) {
		   perror("close");
		   exit(EXIT_FAILURE);
	   }
   }

}

