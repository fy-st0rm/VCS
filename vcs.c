#include "vcs_socket.h"
#include "const.h"

#define PORT 8080
static char* sv_ip = "127.0.0.1";

// vcs init .					; initializes the directory with config files
// vcs new-repo ip:repo-name  ; creates a remote repository in the server

void generate_config()
{
	FILE* fd;
	char* def_config = "HERE SHOULD BE DEFAULT CONFIGS";

	fd = fopen(".vcs", "w");
	if (!fd)
	{
		fprintf(stderr, "[ERROR]: Failed to generate config.\n");
		exit(1);
	}
	fputs(def_config, fd); 
	fclose(fd);
}

int main(int argc, char** argv)
{
	/*
	int client_fd; 

	// Server address
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	inet_pton(AF_INET, ip, &address.sin_addr);

	client_fd = connect_sv(address);
	printf("%d\n", client_fd);
	*/
	
	for (int i = 0; i < argc; i++)
	{
		char* token = argv[i];
		if (!strcmp(token, INIT))
		{
			generate_config();
		}
		else if (!strcmp(token, NEW_REPO))
		{
			token = argv[++i];
			if (!token)
			{
				fprintf(stderr, "[ERROR]: ip and repository name is not provided.\n");
				exit(1);
			}

			// TODO: Add some safety mechanisms here like checking for non alowed symbols in name
			char* ip = strtok(token, ":");
			char* name = strtok(NULL, ":");
			if (!name)
			{
				fprintf(stderr, "[ERROR]: Repository name has not been provided.\n");
				exit(1);
			}

			// Connecting to the server
			int client = connect_sv(ip, PORT);
			char msg[BUFF_CAP] = {0};
			sprintf(msg, "%s %s", NEW_REPO, name);
			send(client, msg, strlen(msg), 0);

			char buffer[BUFF_CAP];
			int res = read(client, buffer, sizeof(buffer));
			if (res <= 0)
				break;

			if (!strcmp(buffer, ERROR))
			{
				printf("ERROR!\n");
			}
			else if (!strcmp(buffer, SUCESS))
			{
				printf("SUCESS\n");
			}
		}
	}
}
