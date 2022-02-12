#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <time.h>
#include <signal.h>

static int somme=0;
static int fd[2];

typedef struct int_s {
	int a;
	int b;
} int_t;

void handler(int signal)
{
	if(signal == SIGINT)
		printf("Somme:%d:\n",somme);
	exit(0);
}

void *random_add(void *arg)
{
	int_t values = *(int_t *)arg;
	free(arg);

	srand(time(NULL));
	while(1)
	{
		int result = values.a + values.b + rand()%10;
		printf("%ld: result:%d\n",pthread_self(), result);
		write(fd[1], &result, sizeof(int));
		sleep(1);
	}
	
	return NULL;
}

int main(void)
{
	struct sigaction action;
	int_t *values, *values2, *values3;
	pthread_t tid;

	action.sa_handler = handler;
	sigaction(SIGINT, &action, NULL);

	pipe(fd);

	values = (int_t *)malloc(sizeof(int_t));
	values->a = 1;
	values->b = 2;

	pthread_create(&tid, NULL, random_add, values);
	pthread_detach(tid);
	
	values2 = (int_t *)malloc(sizeof(int_t));
	values2->a = 3;
	values2->b = 4;

	pthread_create(&tid, NULL, random_add, values2);
	pthread_detach(tid);
	
	values3 = (int_t *)malloc(sizeof(int_t));
	values3->a = 5;
	values3->b = 6;

	pthread_create(&tid, NULL, random_add, values3);
	pthread_detach(tid);
	
	while(1)
	{
		int value,nb_read;
		nb_read = read(fd[0], &value, sizeof(int));
		if(nb_read == sizeof(int))
			somme += value;
		printf("Somme intermediaire:%d:\n", somme);
	}


	return 0;
}
