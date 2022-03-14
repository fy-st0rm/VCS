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
