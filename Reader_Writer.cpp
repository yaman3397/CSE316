#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>

#define MAX 50
int sMod=5;
int rcount=0;
sem_t raccess,baccess;
void *reader(void*);
void *writer(void*);
int main()
{
srand(time(0));
int r,w;
printf("Number of readers : \n");
scanf("%d",&r);
printf("Number of writers : \n");
scanf("%d",&w);
if(r>5)sMod=r;
pthread_t r_t[MAX], w_t[MAX];
sem_init(&raccess,0,1);
sem_init(&baccess,0,1);

int i=0;
for(i=0;i<r;i++)
pthread_create(&r_t[i],NULL,reader,(void*)i);
for(i=0;i<w;i++)
pthread_create(&w_t[i],NULL,writer,(void*)i);
for(i=0;i<w;i++)
pthread_join(w_t[i],NULL);
for(i=0;i<r;i++)
pthread_join(r_t[i],NULL);
}
void *reader(void *r1)
{
int rno=(int)r1+1;
printf("\n Reader %d : Wanting To Read ", rno);
sleep(rand()%sMod);
sem_wait(&raccess);
rcount+=1;
if(rcount==1)
{
sem_wait(&baccess);
}
printf("\n Reader %d : Reading",rno);
sem_post(&raccess);
sleep(rand()%sMod);
sem_wait(&raccess);
rcount-=1;
printf("\n Reader %d : Leaving Reading", rno);
sleep(rand()%sMod);
if(rcount==0)
{
sem_post(&baccess);
}
sem_post(&raccess);
printf("\n Reader %d : Finished",rno);
sleep(rand()%sMod);
return NULL;
}
void *writer(void *w1)
{
int wno=(int)w1+1;
printf("\n Writer %d : Wanting to write",wno);
sleep(rand()%sMod);
sem_wait(&baccess);
printf("\n Writer %d : Writing",wno);
sleep(rand()%sMod);
printf("\n Writer %d : Leaving writing",wno);
sleep(rand()%sMod);
sem_post(&baccess);
printf("\n Writer %d : Finished",wno);
sleep(rand()%sMod);
return NULL;
}
