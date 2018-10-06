#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

#define MAX_PHILOSOPHER 5
#define MAX_SLEEP 3
#define TRUE 1
#define FALSE 0

typedef uint8_t u8;
typedef uint32_t u32;


void *thread(void *params);
void init_philosophers(void);
void test(int i);
void pickup_forks(u32 philosopher_number);
void return_forks(u32 philosopher_number);
void sleep(int);


enum {THINKING=1,EATING} philosophers[MAX_PHILOSOPHER];
pthread_t threads[MAX_PHILOSOPHER];
pthread_mutex_t mutexes[MAX_PHILOSOPHER];
pthread_cond_t cond_vars[MAX_PHILOSOPHER];
pthread_attr_t attr;


void pickup_forks(u32 philosopher_number){
	int i = philosopher_number;
	u8 left=(i+(MAX_PHILOSOPHER-1U))%MAX_PHILOSOPHER;
	u8 right=(i+1U)%MAX_PHILOSOPHER;
	if((THINKING==philosophers[left])&&(THINKING==philosophers[i])&&(THINKING==philosophers[right])){
		printf("\nPhilosophers %d (%d) and philosophers %d (%d) are not eating, ",left,philosophers[left],right,philosophers[right]);
		philosophers[i]=EATING;
		pthread_cond_signal(&cond_vars[i]);
	}
	while(EATING!=philosophers[philosopher_number])
	{
		pthread_cond_wait(&cond_vars[philosopher_number],&mutexes[philosopher_number]);
	}
}

void return_forks(u32 philosopher_number){
	u8 left=(philosopher_number+(MAX_PHILOSOPHER-1U))%MAX_PHILOSOPHER;
	u8 right=(philosopher_number+1)%MAX_PHILOSOPHER;
	philosophers[philosopher_number]=THINKING;
	printf("Philosopher %d finished eating, ", philosopher_number);
	printf("returned its forks, now thinking (%d)!\n",philosophers[philosopher_number]);
	int i=left;
	left=(i+(MAX_PHILOSOPHER-1U))%MAX_PHILOSOPHER;
	right=(i+1U)%MAX_PHILOSOPHER;
	if((THINKING==philosophers[left])&&(THINKING==philosophers[i])&&(THINKING==philosophers[right])){
		printf("\nPhilosophers %d (%d) and %d (%d) are not eating, ",left,philosophers[left],right,philosophers[right]);
		philosophers[i]=EATING;
		pthread_cond_signal(&cond_vars[i]);
	}
	i=right;
	left=(i+(MAX_PHILOSOPHER-1U))%MAX_PHILOSOPHER;
	right=(i+1U)%MAX_PHILOSOPHER;
	if((THINKING==philosophers[left])&&(THINKING==philosophers[i])&&(THINKING==philosophers[right])){
		printf("\nPhilosopher to the left: %d (%d) and to the right: %d (%d) are not eating, ",left,philosophers
							[left],right,philosophers[right]);
		philosophers[i]=EATING;
		pthread_cond_signal(&cond_vars[i]);
	}
}


void *thread(void *params){
	u32 philosopher = (u32)	 params;
	u8 left=(philosopher+(MAX_PHILOSOPHER-1U))%MAX_PHILOSOPHER;
	u8 right=(philosopher+1U)%MAX_PHILOSOPHER;
	pthread_mutex_lock(&mutexes[philosopher]);
	do{
		printf("Philosopher on position %d is trying to eat...\n",philosopher);
		pickup_forks(philosopher);
		printf("Philosofer %d (%d) is eating now!\n",philosopher,philosophers[philosopher]);
		sleep((rand() % MAX_SLEEP)+1U);		
		return_forks(philosopher);
		sleep((rand() % MAX_SLEEP)+1U);		
	}while(TRUE);
	pthread_exit(FALSE);
}

int main(void){
	pthread_attr_init(&attr);
	u8 i;
	void *ptr;
	for(i=0;i<MAX_PHILOSOPHER;i++){
		pthread_mutex_init(&mutexes[i],NULL);
		pthread_cond_init(&cond_vars[i],NULL);
		philosophers[i] = THINKING;
	}
	for(i=0;i<MAX_PHILOSOPHER;i++)
	{
		ptr = &i;
		 pthread_create(&threads[i],&attr,thread,(void *) i);
	}
	for(i=0;i<MAX_PHILOSOPHER;i++)
	{
		pthread_join(threads[i],NULL);
	}
	printf("\nPhilosophers are done eating!\n");
	return 0;
}
