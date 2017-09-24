#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

int main ()
{
	char* buf = (char*)malloc(4096*sizeof(char));
	char* buf1 = (char*)malloc(4096*sizeof(char));
	int i = 0;
	FILE* fp = fopen("in.txt", "r");
	FILE* fv = fopen("out.txt", "w");
	if ((fp == NULL) || (fv == NULL))
	{
		perror("Can't open file");
		exit(-1);
	}
	while (!feof(fp))
	{
		fgets(buf, sizeof(buf), fp);
		for (i = 0; i < sizeof(buf); i++)
		{
			buf1[i] = toupper(buf[i]);
		}
		fputs(buf1, fv);
	}
	fclose(fp);
	fclose(fv);
	free(buf);
	free(buf1);
	return 0;
}
