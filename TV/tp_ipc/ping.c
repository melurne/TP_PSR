#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NB 10
#define CLE_MSG (key_t)1000
#define TAILLE_MSG 20

typedef struct {
	long type;
	char texte[TAILLE_MSG];
} Msg_requette_t;

int msgid;


int main(void) {
	Msg_requette_t message;

	if ((msgid = msgget(CLE_MSG, 0666)) == -1)
	{
		perror("msgget");
		exit(1);
	}

	message.type=1;
	strcpy(message.texte, "ping");
		
	time_t begin = clock();
	msgsnd(msgid, &message, TAILLE_MSG, 0);
		
	msgrcv(msgid, &message, TAILLE_MSG, 2, 0);
	float diff = (clock() - begin)/CLOCKS_PER_SEC *1000;
	printf("Temps d'aller-retour : %f\n", diff);

	return 0;
}
