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
	int len = 0;
	key_t k;
	int msgid;
	mymsg msg1;
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
	while (1)
	{
		if ((len = msgrcv(msgid, (mymsg*)&msg1, LEN, 0, 0)) < 0)
		{
			perror("msgrcv");
			exit(-2);
		}
		if (len == 0)
		{
			printf("No messages\n");
			exit(0);
		}
		if (msg1.mtype == 255)
		{
			printf("type = %ld, data = %s\n", msg1.mtype, msg1.mdata);
			if (msgctl(msgid, IPC_RMID, (struct msqid_ds*)NULL))
			{
				perror("msgctl");
				exit(-3);
			}
			exit(0);
		}
		printf("type = %ld, data = %s\n", msg1.mtype, msg1.mdata);
	}
	return 0;
}
