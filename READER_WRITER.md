#include<stdio.h>

#include<pthread.h>

#include<semaphore.h>

sem_t mutex,rw_mutex;
int data = 5,rcount = 0;
void *reader(void *arg)
{

  int f;

  f = ((int)arg);

  sem_wait(&mutex);

  rcount = rcount + 1;

  if(rcount==1)

   sem_wait(&rw_mutex);

  sem_post(&mutex);

  printf("Data read by the reader%d is %d\n",f,data);

  sleep(5);

  sem_wait(&mutex);

  rcount = rcount - 1;

  if(rcount==0)

   sem_post(&rw_mutex);

  sem_post(&mutex);
pthread_exit("reader exit\n");

}
