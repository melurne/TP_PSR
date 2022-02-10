#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NB 10
#define CLE_MSG (key_t)1000
#define TAILLE_MSG sizeof(Msg_requette_t) - sizeof(long)

typedef struct coupleFloats {
	float a;
	float b;
} coupleFloats;

typedef struct coupleInts {
	int a;
	int b;
} coupleInts;

typedef struct {
	long type;
	//char texte[TAILLE_MSG];
	coupleFloats para_multi;
	coupleInts para_add;
	int result_add;
	float result_multi;
} Msg_requette_t;

int msgid;

int main(void) {
	Msg_requette_t message;

	if ((msgid = msgget(CLE_MSG, 0666)) == -1)
	{
		perror("msgget");
		exit(1);
	}

	while (1)
	{
		int op;
		coupleFloats para_multi;
		coupleInts para_add;
		
		printf("0 -> add, 1 -> multi : ");
		scanf("%d", &op);
		
		if (op == 0) {
			scanf("%d", &(para_add.a));
			scanf("%d", &(para_add.b));
			message.type=10;
			message.para_add = para_add;
			msgsnd(msgid, &message, TAILLE_MSG, 0);
			msgrcv(msgid, &message, TAILLE_MSG, 30, 0);

			printf("Returned : %d\n", message.result_add);
		} 
		else {
			scanf("%f", &(para_multi.a));
			scanf("%f", &(para_multi.b));
			message.type=20;
			message.para_multi = para_multi;
			msgsnd(msgid, &message, TAILLE_MSG, 0);
			msgrcv(msgid, &message, TAILLE_MSG, 30, 0);

			printf("Returned : %f\n", message.result_multi);
		}
	}
}
