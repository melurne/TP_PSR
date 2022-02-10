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

void handler(int sig) {
	if (sig == SIGINT) {
		printf("Aurevoir\n");
		msgctl(msgid, IPC_RMID, 0);
		exit(0);
	}
}

void * multiplier(void * parameter) {
	coupleFloats * couple = (coupleFloats *)parameter;
	float a = couple->a, b = couple->b;
	float * out = malloc(sizeof(float));
	*out = a*b;
	return (void *)out;
}

void * adder(void * parameter) {
	coupleInts * couple = (coupleInts *)parameter;
	int a = couple->a, b = couple->b;
	int * out = malloc(sizeof(int));
	*out = a+b;
	return (void *)out;
}

int main(void) {
	Msg_requette_t message;

	struct sigaction act;
	act.sa_handler = &handler;

	sigaction(SIGINT, &act, NULL);

	pthread_t id;
	//coupleFloats para_multi;
	//coupleInts para_add;
	void * return_val;

	if ((msgid = msgget(CLE_MSG, IPC_CREAT|0666)) == -1)
	{
		perror("msgget");
		exit(1);
	}

	while (1)
	{
		msgrcv(msgid, &message, TAILLE_MSG, -21, 0);
		if (message.type == 10)
		{
			coupleInts para_add = message.para_add; 
			pthread_create(&id, NULL, &adder, &para_add);
			pthread_join(id, &return_val);
			
			message.type=30;
			//memcpy(&message.texte, &(char *)*(coupleInts *)(return_val), sizeof(coupleInts));
			message.result_add = *(int *)return_val;
			msgsnd(msgid, &message, TAILLE_MSG, 0);
			
			free(return_val);
		}
		else if (message.type == 20) 	
		{
			coupleFloats para_multi = message.para_multi; 
			pthread_create(&id, NULL, &multiplier, &para_multi);
			pthread_join(id, &return_val);
			
			message.type=30;
			//message.texte = (char *)*(coupleFloats *)return_val;
			//memcpy(&message.texte, &(char *)*(coupleFloats *)(return_val), sizeof(coupleFloats));
			message.result_multi = *(float *)return_val;
			msgsnd(msgid, &message, TAILLE_MSG, 0);
			
			free(return_val);
		}
	}
}
