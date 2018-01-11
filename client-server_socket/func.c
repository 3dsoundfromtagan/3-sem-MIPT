#include <string.h>
#include <stdio.h>

void correct (char* buf)
{
	int i = 0;
	int n = strlen(buf);
	for (i = 0; i < n - 2; i++)
	{
		buf[i] = buf[i + 1];
	}
	buf[n - 2] = '\0';
}

int main()
{
	printf("*************************\n");
	char buf[100500];
	char mas[100];
	scanf("%s", mas);
	fgets(buf, sizeof(buf), stdin);
	printf("len =  %d\n", (int)strlen(buf));
	printf("before: %s\n", buf);
	correct(buf);
	printf("after: %s\n", buf);
	printf("*************************\n");
	return 0;
}

