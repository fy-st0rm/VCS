#include "util.h"

bool file_exists(char* path, char* file)
{
	DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(!strcmp(file, dir->d_name))
				return true;
        }
        closedir(d);
    }
	return false;
}

void recur_list_dir(char* path, char** in, char*** out, int* out_idx)
{
	DIR* d;
	struct dirent* dir;
	d = opendir(path);
	if (d)
	{
        while ((dir = readdir(d)) != NULL)
        {
			if (dir->d_type == DT_DIR)
			{
				if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
				{
					char new_path[strlen(path) + strlen(dir->d_name) + 2];
					strcpy(new_path, path);
					strcat(new_path, "/");
					strcat(new_path, dir->d_name);
					recur_list_dir(new_path, in, out, out_idx);
				}
			}
			else
			{
				// Forming the file name with the relative path 
				char name[strlen(path) + strlen(dir->d_name) + 2];
				strcpy(name, path);
				strcat(name, "/");
				strcat(name, dir->d_name);
				
				//TODO: FIX THIS REALLOC CAUSING ERROR OF FREEING OR SOMETHING ;-;
				//TODO: THIS SZ ONLY WORKS FOR THE SMALLER DIRECTORY MAKE IT VERSATILE
				
				// Increasing the size of the buffer
				int sz = sizeof(in) * 200; //(*out_idx + 1) * 200;
				in = realloc(in, sz);
				in[*out_idx] = calloc(sz, sizeof(char));

				// copying into the input buffer
				strcpy(in[*out_idx] , name);
				*out_idx += 1;
			}
        }
        closedir(d);
	}
	// Giving the pointer to the output buffer
	*out = in;
}

char* load_file(char* file)
{
	FILE* fd;
	fd = fopen(file, "r");
	if (fd == NULL)
	{
		fprintf(stderr, "Failed to open file `%s`.\n", file);
		exit(1);
	}

	int sz = 1;
	char ch;
	do {
		ch = fgetc(fd);
		sz++;
	} while (ch != EOF);  
	fclose(fd);

	fd = fopen(file, "r");
	if (fd == NULL)
	{
		fprintf(stderr, "Failed to open file `%s`.\n", file);
		exit(1);
	}
	char* buff = calloc(sz, sizeof(char));

	int i = 0;
	while ((ch = fgetc(fd)) != EOF)
	{
		buff[i++] = ch;
	}
	buff[i] = '\0';

	fclose(fd);
	return buff;
}

