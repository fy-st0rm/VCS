#ifndef _CONST_H_
#define _CONST_H_

// STD includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

// Path based includes
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

// Networking includes
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Networking constants
#define BUFF_CAP 1024
#define SV_DIR   ".vcs_servers"
#define DELAY    0.1

// Tokens
#define INIT       "init"
#define NEW_REPO   "new-repo"
#define STATUS     "status"
#define REPO_NAME  "REPO_NAME"
#define SV_IP      "SERVER_IP"

// FLAGS
#define ERROR	"error" 
#define SUCESS  "sucess"

#endif        
