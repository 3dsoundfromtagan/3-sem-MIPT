#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define PATH 1000

//Обход директорий по рекурсии
//Пример - директория mydir
typedef struct _count
{
	int k;
	int k_nolink;
} count; 
count c;
int all_links = 0;;

count file_count (char* path)
{
	struct dirent *read_dir;
	DIR* dirp = opendir(path);
	if (dirp == NULL)
	{
		perror("opendir");
		exit(-1);
	}	
	
	while ((read_dir = readdir(dirp)) != NULL)
	{
		if (read_dir->d_type != DT_DIR)
		{
			c.k++;
			if (read_dir->d_type != DT_LNK)
			{
				c.k_nolink ++;
			}
		}
		if ((read_dir->d_type == DT_DIR) && (strcmp(read_dir->d_name, ".") != 0) && (strcmp(read_dir->d_name, "..") != 0))
		{
			strcat(path, "/");
			strcpy(path, strcat(path, read_dir->d_name));
			printf("%s\n", path);
			file_count(path);
		}
	}
	closedir(dirp);
	return c;
}


int main ()
{
	char path[PATH];
	printf("Input path:\n");
	scanf("%s", path);
	count c;
	c = file_count(path);
	printf("%d files, %d files without links\n", c.k, c.k_nolink);	
	return 0;
	
}
