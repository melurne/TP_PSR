#include <stdio.h>
#include <pthread.h>


static int globale=0;

#ifdef SEMAPHORE
pthread_mutex_t myMutex;
#endif


void *thread1(void *arg)
{
#ifdef SEMAPHORE
	pthread_mutex_lock(&myMutex);
#endif
	globale += 500;
	printf("Thread1: globale:%d:\n",globale);
#ifdef SEMAPHORE
	pthread_mutex_unlock(&myMutex);
#endif
	return NULL;
}


void *thread2(void *arg)
{
#ifdef SEMAPHORE
	pthread_mutex_lock(&myMutex);
#endif
	globale /= 2;
	printf("Thread2: globale:%d:\n",globale);
#ifdef SEMAPHORE
	pthread_mutex_unlock(&myMutex);
#endif
	return NULL;
}


int main(void)
{
	pthread_t tid1, tid2;

#ifdef SEMAPHORE
	pthread_mutex_init(&myMutex, NULL);
#endif

	pthread_create(&tid1, NULL, thread1, NULL);
	pthread_create(&tid2, NULL, thread2, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	printf("Apres terminaison des threads, globale=%d\n",globale);

	return 0;
}
