#include "vcs_socket.h"
#include "util.h"
#include "const.h"

#define PORT 8080

typedef struct
{
	char* path;
	int conn;
} Args;

void handle_client(Args* arg)
{
	int conn = arg->conn;
	while (1)
	{
		char buffer[BUFF_CAP] = {0};
		int res = read(conn, buffer, sizeof(buffer));
		if (res <= 0)
			break;
		printf("received: %d %s\n", res, buffer);

		char* token = strtok(buffer, " ");
		if (!strcmp(token, NEW_REPO))
		{
			char* file = strtok(NULL, " ");

			// Check if the repo exists or not.
			bool is_file = file_exists(arg->path, file);
			if (is_file) // If already exists
			{
				sleep(DELAY);
				send(conn, ERROR, sizeof(ERROR), 0);	
				break;
			}
			else
			{
				// Create new repository directory
				char full_path[strlen(arg->path) + strlen(file) + 1];
				strcpy(full_path, arg->path);
				strcat(full_path, "/");
				strcat(full_path, file);
				mkdir(full_path, 0777);

				// Sending some feedback towards the client
				sleep(DELAY);
				send(conn, SUCESS, sizeof(SUCESS), 0);	
			}
		}
		else if (!strcmp(token, STATUS))
		{
			char* repo = strtok(NULL, " ");
			printf("ASKED FOR REPO: %s\n", repo);

			char path[strlen(repo) + 3];
			strcpy(path, arg->path);
			strcat(path, "/");
			strcat(path, repo);

			// Getting all the files from that repo
			char** test = NULL;
			int idx = 0;
			recur_list_dir(path, test, &test, &idx);
			for (int i = 0; i < idx; i++)
			{
				printf("%s\n", test[i]);
			}

			// TODO: check for deleted files
			// TODO: check for modified files
		}
	}
	printf("%d disconnected.\n", conn);
	close(conn);
}

void create_sv_dir(char* path)
{
	bool is_file = file_exists(path, SV_DIR);

	// When the server directory doesnt exists
	if (!is_file)
	{
		chdir(path);
		mkdir(SV_DIR, 0777);
		chdir(SV_DIR);
		printf("[INFO]: New server directory created at %s as %s\n", path, SV_DIR);
	}
}

int main()
{
	// Getting the home path of the system
	struct passwd *passwdEnt = getpwuid(getuid());
	char *home = passwdEnt->pw_dir;
	create_sv_dir(home);

	// Creating sv path to the server folder
	char sv_path[strlen(home) + strlen(SV_DIR)];
	strcpy(sv_path, home);
	strcat(sv_path, "/");
	strcat(sv_path, SV_DIR);

	int server_fd;

	// Server address
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Creating server 
	server_fd = create_server(address, 1);

	pthread_t thread;
	int conn;
	int addrlen = sizeof(address);
	
	// Creating arguments
	while (1)
	{
		// Listening for the clients
		listen(server_fd, 0);

		if ((conn = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &addrlen)) < 0)
			fprintf(stderr, "Accept error!");
		Args arg = {sv_path, conn};
		int err = pthread_create(&thread, NULL, (void*)handle_client, (void*) &arg);
		if (err)
			fprintf(stderr, "Error in threading!");
	}

	return 0;
}
