#include <pthread.h>

int bar;

void* foo1(void * p) {
	int * val = (int *)p;
	*val = *val+500;
	printf("(t1) La valeur de bar est : %d\n", *val);
}

void* foo2(void * p) {
	int * val = (int *)p;
	*val = *val/2;
	printf("(t2) La valeur de bar est : %d\n", *val);

}


int main(void) {
	pthread_t id1, id2;
	pthread_create(&id1, NULL, &foo1, &bar);
	pthread_create(&id2, NULL, &foo2, &bar);
	
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);
	return 0;
}
