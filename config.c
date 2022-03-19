#include "config.h"

void generate_config()
{
	FILE* fd;
	char* def_config = "REPO_NAME=new repo";

	fd = fopen(".vcs", "w");
	if (!fd)
	{
		fprintf(stderr, "[ERROR]: Failed to generate config.\n");
		exit(1);
	}
	fputs(def_config, fd); 
	fclose(fd);
}

void read_config(Dict* dict)
{
	char* file = load_file(".vcs");

	char lines[strlen(file)][strlen(file)];
	char* line = strtok(file, "\n");
	int idx = 0;
	while (line)
	{
		strcpy(lines[idx++], line);
		line = strtok(NULL, "\n");
	}

	for (int i = 0; i < idx; i++)
	{
		char* token = strtok(lines[i], "=");
		char* value = strtok(NULL, "=");
		dict_insert(dict, token, value, strlen(token) * sizeof(char), strlen(value) * sizeof(char));
	}
}

void save_config(Dict* dict)
{
	FILE* fd;

	fd = fopen(".vcs", "w");
	if (!fd)
	{
		fprintf(stderr, "[ERROR]: Failed to generate config.\n");
		exit(1);
	}

	for (int i = 0; i < dict->len; i++)
	{
		Dict_item* item = dict_get_item_idx(dict, i);
		fputs(item->key, fd); 
		fputs("=", fd);
		fputs(item->value, fd);
		fputs("\n", fd);
	}
	fclose(fd);
}
