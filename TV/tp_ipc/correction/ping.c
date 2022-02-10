#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "pingpong.h"

static int msgid;


void handlerSigint(int sig)
{
	if(sig==SIGINT)
	{
		msgctl(msgid, IPC_RMID, 0);
		printf("Au revoir\n");
	}
	exit(SIGINT);
}

int main(void)
{
	pongmsg_t message;
	struct sigaction action;

	if((msgid=msgget(KEY,IPC_CREAT|IPC_EXCL|0666))==-1)
	{
		perror("msgget");
		exit(1);
	}

	action.sa_handler=handlerSigint;
	sigaction(SIGINT,&action,NULL);

	while(1)
	{
		msgrcv(msgid,&message,MSG_SIZE,1,0);
		printf("Message recu:%s:\n",message.msg);
		message.type=2;
		strcpy(message.msg,"Reponse");
		msgsnd(msgid,&message,MSG_SIZE,0);		
	}

	return 0;
}
