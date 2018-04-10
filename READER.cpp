#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define SIZE_BUFF 10		/* Size of shared b */

int b[SIZE_BUFF];  	/* shared b */
int r = 0;  			/* place to add next element */
int fr = -1; 			/* place to remove next element */
int c = 0;  		/* number elements in b */

int raccess = 0;
int rPriorityFlag = 0;     /* This flag is set to 1 when readers access the resources so that writers do not access it*/
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; 	/* mutex lock for b */
pthread_cond_t cons_cond = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t prod_cond = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */
pthread_mutex_t mtxData = PTHREAD_MUTEX_INITIALIZER; /* mutex lock for data b */
void *writer(void* param);
void *reader(void* param);
void *writer(void* param)
{	
	int r = rand() % 500;
	//fprintf(stdout, "Sleep for %d\n", r);
	usleep(r);
	//fprintf(stdout, "Thread writer\n");
	pthread_mutex_lock(&mtx);
		while(raccess > 0 || rPriorityFlag == 1)
			pthread_cond_wait(&prod_cond,&mtx);
		--raccess;
	pthread_mutex_unlock(&mtx);
	
	// write data here	
	unsigned int tid = (unsigned int)pthread_self();

	pthread_mutex_lock(&mtxData);

	if (fr != r)  // check if b is not full
	{
		int newVal = rand() % 300;
		b[r] = newVal; 
		r = (r + 1) %  SIZE_BUFF; // set new position	
		int readersC = raccess < 0 ? 0 : raccess;
		fprintf(stdout, "Data written by thread %u is %d with readers %d\n", tid, newVal, readersC);
	}

	pthread_mutex_unlock(&mtxData);
	
	pthread_mutex_lock(&mtx);
		++raccess;
		pthread_cond_broadcast(&cons_cond);
		pthread_cond_broadcast(&prod_cond);
	pthread_mutex_unlock(&mtx);

}

void *reader(void* param)
{	
	int r = rand()%(500);
	usleep(r);
	pthread_mutex_lock(&mtx);
		if(raccess < 0) 
		{
			rPriorityFlag = 1;
		}
		else
		{
			++raccess;
		}
	pthread_mutex_unlock(&mtx);
	pthread_mutex_lock(&mtxData);
	if ((fr + 1) % SIZE_BUFF != r)
	{
		fr = (fr + 1) % SIZE_BUFF;
		int val = b[fr];
		unsigned int tid = (unsigned int)pthread_self();
		fprintf(stdout, "Data read by thread %u\n is %d readers %d\n", tid, val, raccess);	
	}
		
	pthread_mutex_unlock(&mtxData);		
	pthread_mutex_lock(&mtx);
		--raccess;
		rPriorityFlag = 0;
		pthread_cond_broadcast(&cons_cond);
		pthread_cond_broadcast(&prod_cond);
	pthread_mutex_unlock(&mtx);

}
int main(int argc, char *argv[]) 
{
	srand(time(NULL));
	pthread_t t[SIZE_BUFF];
	int i;
	for (i = 0; i < SIZE_BUFF / 2; ++i)
	{
	  if(pthread_create(&t[i], NULL, writer, NULL) != 0)
	  {
		fprintf(stderr, "Unable to create writer thread\n");
		exit(1);
	  }
	  if(pthread_create(&t[i + SIZE_BUFF/2], NULL, reader, NULL) != 0)
	  {
		fprintf(stderr, "Unable to create reader thread\n");
		exit(1);
	  }
	}
	for (i = 0; i < SIZE_BUFF; ++i)
	{
		pthread_join(t[i], NULL);	
	}
	fprintf(stdout, "Parent thread quitting\n");
	return 0;
}


