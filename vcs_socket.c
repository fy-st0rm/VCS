#include "vcs_socket.h"
#include <stdlib.h>
#include <stdio.h>


int create_server(struct sockaddr_in address, int opt)
{
	int server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		fprintf(stderr, "Socket failed!\n");
		exit(1);
	}

	// To reuse the address
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		fprintf(stderr, "Sockopt failed!\n");
		exit(1);
	}
	
	// Binding
	if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0)
	{
		fprintf(stderr, "Failed to bind sv!\n");
		exit(1);
	}
	return server_fd;
}

int connect_sv(char* ip, int port)
{
	int server;
	if ((server = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		fprintf(stderr, "[Error]: Socket failed! Check your connection and try again later.\n");
		exit(1);
	}

	// Setting Non block
	/*
	 * NOTE: Fixes the hang issue but the connect function will always returns -1
	int rv = fcntl(server, F_SETFL, O_NONBLOCK);
	if(rv < 0) 
	{
		fprintf(stderr, "[ERROR]: nonblock failed: %d %s\n", errno, strerror(errno));
		exit(1);
	}
	*/

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, ip, &address.sin_addr);

	// Trying to connect to the server
	// TODO: Fix the hang issue when the server ip is not online
	if (connect(server, (struct sockaddr*) &address, sizeof(address)) < 0)
	{
		fprintf(stderr, "[ERROR]: Failed to connect to the remote server. Server might be offline or check your connection and try again later.\n"); 
		exit(1);
	}
	return server;
}
