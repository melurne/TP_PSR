#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "pingpong.h"


int main(void)
{
	int msgid;
	pongmsg_t message;

	if((msgid=msgget(KEY,0666))==-1)
	{
		perror("msgget");
		exit(1);
	}

	message.type=1;
	strcpy(message.msg,"Question");
	msgsnd(msgid,&message,MSG_SIZE,0);		
	msgrcv(msgid,&message,MSG_SIZE,2,0);
	printf("Message recu:%s:\n",message.msg);

	return 0;
}
