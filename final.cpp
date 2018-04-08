#include<stdio.h>

#include<pthread.h>

#include<semaphore.h>



sem_t mutex,rw_mutex;

int data = 5,rcount = 0;



void *reader(int arg)

{

  int f;

  f = arg;

  sem_wait(&mutex);

  rcount = rcount + 1;

  if(rcount==1)

   sem_wait(&rw_mutex);

  sem_post(&mutex);

  printf("Data read by the reader%d is %d\n",f,data);

  sleep(2);

  sem_wait(&mutex);

  rcount = rcount - 1;

  if(rcount==0)

   sem_post(&rw_mutex);

  sem_post(&mutex);
pthread_exit("reader exit\n");

}



void *writer(void *arg)

{

  int f;

  f =  arg;

  sem_wait(&rw_mutex);

  data++;

  printf("Data writen by the writer%d is %d\n",f,data);

  sleep(1);

  sem_post(&rw_mutex);
    pthread_exit("writer exit");

}



int main()

{

  int i,c;


  pthread_t p1[5],p2[5];

  sem_init(&mutex,0,1);

  sem_init(&rw_mutex,0,1);
	void *status;
	for(int i=0;i<5;i++){
		pthread_create(&p1[i],NULL,writer,i);
		sleep(2);
	}
	for(int i=0;i<5;i++){
		pthread_create(&p2[i],NULL,reader,i);
	}
	for(int i=0;i<5;i++){
		pthread_join(p1[i],NULL);
	}
	for(int i=0;i<5;i++){
		pthread_join(p2[i],NULL);
	}

  return 0;

}


