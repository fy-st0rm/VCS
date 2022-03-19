#include "vcs_socket.h"
#include "const.h"
#include "dict.h"
#include "config.h"

#define PORT 8080
static char* sv_ip = "127.0.0.1";

//TODO: [ ] Some sort of diff tracker
//TODO: [ ] Read and write of the config

// [X] vcs init .					; initializes the directory with config files
// [X] vcs new-repo ip:repo-name  ; creates a remote repository in the server
// [ ] vcs status
// [ ] vcs add [file]
// [ ] vcs commit [msg]
// [ ] vcs push

int main(int argc, char** argv)
{
	Dict* config = dict_new(100);
	// Parsing the cmd line arguments
	for (int i = 0; i < argc; i++)
	{
		char* token = argv[i];
		if (!strcmp(token, INIT))
		{
			generate_config();
			read_config(config);
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
				printf("[ERROR]: Repository with name `%s` already exists. Please try another name.\n", name);
			}
			else if (!strcmp(buffer, SUCESS))
			{
				// Generating repo name in config file
				size_t k_sz = strlen(REPO_NAME) * sizeof(char);
				size_t v_sz = strlen(name) * sizeof(char);

				// Saving the server ip
				size_t s_sz = strlen(SV_IP) * sizeof(char);
				size_t i_sz = strlen(ip) * sizeof(char);

				read_config(config);
				dict_pop(config, (void*) REPO_NAME, k_sz);  
				dict_insert(config, (void*) REPO_NAME, (void*) name, k_sz, v_sz);
				dict_insert(config, (void*) SV_IP, (void*) ip, s_sz, i_sz);
				save_config(config);

				printf("[SUCESS]: Sucessfully create a repository with name `%s`.\n", name);
			}
		}
		else if (!strcmp(token, STATUS))
		{
			// getting all the files to get the untracked files 
			char** test = NULL;
			int idx = 0;
			recur_list_dir(".", test, &test, &idx);

			// Loading the config
			read_config(config);
			dict_print(config);
			char* repo_name = dict_get(config, (void*) REPO_NAME, strlen(REPO_NAME) * sizeof(char));
			char* sv_ip = dict_get(config, (void*) SV_IP, strlen(SV_IP) * sizeof(char));

			// TODO: find the modified files

			// Ask to the server
			int client = connect_sv(sv_ip, PORT);
			char msg[BUFF_CAP] = {0};
			sprintf(msg, "%s %s", STATUS, repo_name);
			send(client, msg, strlen(msg), 0);
		}
	}

	dict_clean(config);
}
