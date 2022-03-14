#ifndef _VCS_SOCKET_H_ 
#define _VCS_SOCKET_H_ 

#include "const.h"

int create_server(struct sockaddr_in address, int opt);
int connect_sv(char* ip, int port);

#endif
