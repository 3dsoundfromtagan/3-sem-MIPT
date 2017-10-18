#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

int main ()
{
	char* buf = (char*)malloc(4096*sizeof(char));
	char* buf_1 = (char*)malloc(4096*sizeof(char));
	int i = 0;
	FILE* fi = fopen("in.txt", "r");
	FILE* fo = fopen("out.txt", "w");
	if ((fi == NULL) || (fo == NULL))
	{
		perror("Can't open files");
		exit(-1);
	}
	while(!feof(fi))	
	{
		fgets(buf, sizeof(buf), fi);
		if(!feof(fi))
		{
			for (i = 0; i < sizeof(buf); i++)
			{
				buf_1[i] = toupper(buf[i]);
			}	
			fputs(buf_1, fo);
		}
	}
	fclose(fi);
	fclose(fo);
	free(buf);
	free(buf_1);
	return 0;
}
