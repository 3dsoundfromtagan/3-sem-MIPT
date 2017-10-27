#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LEN	20
#define N	5

typedef struct _mymsg
{
	long mtype;
	char mdata[LEN];
} mymsg;

int main ()
{
	int i = 0;
	int len = 0;
	key_t k;
	int msgid;
	mymsg msg1;
	msg1.mtype = 1;
	strcpy(msg1.mdata, "HelloWorld");
	//Get key
	if ((k = ftok("./ex1", 1)) < 0)
	{
		perror("ftok");
		exit(-1);
	}
	//Get access
	if ((msgid = msgget(k, 0666 | IPC_CREAT)) < 0)
	{
		perror("msgget");
		exit(-2);
	}
	//Make N messages
	for(i = 0; i < N; i++)
	{
		len = strlen(msg1.mdata) + 1;
		if(msgsnd(msgid, (mymsg*)&msg1, len, 0) < 0)
		{
			perror("msgsnd");
			exit(-3);
		}
		printf("type = %ld, data = %s\n", msg1.mtype, msg1.mdata);
	}
	msg1.mtype = 255;
	strcpy(msg1.mdata, "");
	if(msgsnd(msgid, (mymsg*)&msg1, len, 0) < 0)
	{
		perror("msgsnd");
		exit(-3);
	}
	printf("type = %ld, data = %s\n", msg1.mtype, msg1.mdata);
	return 0;
}
