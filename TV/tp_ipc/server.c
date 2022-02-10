#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define NB 10
#define CLE_MSG (key_t)1000
#define TAILLE_MSG 20

typedef struct {
	long type;
	char texte[TAILLE_MSG];
} Msg_requette_t;

int msgid;

void handler(int sig) {
	if (sig == SIGINT) {
		printf("Aurevoir\n");
		msgctl(msgid, IPC_RMID, 0);
		exit(0);
	}
}

int main(void) {
	Msg_requette_t message;

	struct sigaction act;
	act.sa_handler = &handler;

	sigaction(SIGINT, &act, NULL);

	if ((msgid = msgget(CLE_MSG, IPC_CREAT|0666)) == -1)
	{
		perror("msgget");
		exit(1);
	}

	while (1)
	{
		msgrcv(msgid, &message, TAILLE_MSG, 1, 0);
		message.type=2;
		strcpy(message.texte, "pong");
		msgsnd(msgid, &message, TAILLE_MSG, 0);
	}
}
