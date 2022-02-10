#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct arguments {
	float a;
	float b;
} arguments;

int fd[2];
float total = 0;

void handler(int arg) {
	if (arg == SIGINT) {
		printf("Total = %f\n", total);
		exit(NULL);
	}
}

void * random_bot(void * arg) {
	arguments * p = (arguments *)arg;
	float para_int = p->a + p->b;
	float out;
	//FILE * pipe = fdopen(fd[1], "a+");
	srand(time(NULL));
	while (1) {
		sleep(1);
		out = para_int + rand(); 
		write(fd[1], out, sizeof(int));
	}
}

void * adder(void * arg) {
	float buf;
	while (1) {
		read(fd[0], &buf, sizeof(int));
		total = total+buf;
	}
}

int main(void) {
	struct sigaction act;
	act.sa_handler = &handler;

	sigaction(SIGINT, &act, NULL);

	pthread_t id1, id2, id3, id4;
	pipe(fd);

	arguments arg1 = {1, 2}, arg2 = {3, 4}, arg3 = {5, 6};
	pthread_create(&id1, NULL, &random_bot, (void *)&arg1); pthread_detach(id1);
	pthread_create(&id2, NULL, &random_bot, (void *)&arg2);	pthread_detach(id2);
	pthread_create(&id3, NULL, &random_bot, (void *)&arg3); pthread_detach(id3);
	
	pthread_create(&id4, NULL, &adder, NULL); pthread_detach(id4);


}

