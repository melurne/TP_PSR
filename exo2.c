#include <pthread.h>
#include <stdio.h>
#include <signal.h>

typedef struct coupleFloats {
	float a;
	float b;
} coupleFloats;

typedef struct coupleInts {
	int a;
	int b;
} coupleInts;

void handler(int arg) {
	if (arg == SIGINT) {
		printf("Quitting\n");
		exit(NULL);
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
	struct sigaction act;
	act.sa_handler = &handler;

	sigaction(SIGINT, &act, NULL);

	int op;
	pthread_t id;
	coupleFloats para_multi;
	coupleInts para_add;
	void * return_val;
	while (1) {
		printf("0 -> add, 1 -> multi : ");
		scanf("%d", &op);
		
		if (op == 0) {
			scanf("%d", &(para_add.a));
			scanf("%d", &(para_add.b));
			pthread_create(&id, NULL, &adder, &para_add);
			pthread_join(id, &return_val);
			printf("Returned : %d\n", *(int *)return_val);
			free(return_val);
		} 
		else {
			scanf("%f", &(para_multi.a));
			scanf("%f", &(para_multi.b));
			pthread_create(&id, NULL, &multiplier, &para_multi);
			pthread_join(id, &return_val);
			printf("Returned : %f\n", *(float *)return_val);
			free(return_val);
		}
	}
}

