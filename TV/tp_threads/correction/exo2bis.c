#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <pthread.h>

typedef struct args_add_s {
	int arg1;
	int arg2;
} args_add_t;

typedef struct args_mul_s {
	float arg1;
	float arg2;
} args_mul_t;

void handler(int sig)
{
	if(sig == SIGINT)
	{
		printf("\nAu revoir\n");
		exit(0);
	}
	exit(0);
}

void *threadAdd(void *arg)
{
	printf("Resultat de l'addition : %d+%d=%d\n",((int *)arg)[0],((int *)arg)[1],((int *)arg)[0]+((int *)arg)[1]);
	return NULL;
}

void *threadMul(void *arg)
{
	//args_mul_t arg_s = *(args_mul_t)arg;
	args_mul_t arg_s;
	memcpy(&arg_s, arg, sizeof(args_mul_t));
	printf("Résultat de la multiplication : %f*%f=%f\n", arg_s.arg1, arg_s.arg2, arg_s.arg1*arg_s.arg2);

	return NULL;
}

int main(void)
{
	int op; /* operation */
	int tab[2];
	args_mul_t mul;

	struct sigaction action;
	action.sa_handler = handler;
	sigaction(SIGINT, &action, NULL);

	while(1)
	{
		printf("1: addition\n2:multiplication\n");
		scanf("%d",&op);
		if(op == 1)
		{
			pthread_t tid;
			printf("Entrez la premiere valeur\n");
			scanf("%d",&tab[0]);
			printf("Entrez la deuxieme valeur\n");
			scanf("%d",&tab[1]);
			pthread_create(&tid, NULL, threadAdd, (void *)tab);
			pthread_detach(tid);
		}
		else if( op == 2)
		{
			pthread_t tid;
			printf("Entrez la premiere valeur\n");
			scanf("%f",&(mul.arg1));
			printf("Entrez la deuxieme valeur\n");
			scanf("%f",&(mul.arg2));
			pthread_create(&tid, NULL, threadMul, (void *)&mul);
			pthread_join(tid,NULL);
		}
	}
	return 0;
}
