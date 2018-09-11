#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


struct Thread{
	long ID;
	long iteration;
};
typedef struct Thread mythread;

static int num_thread;
static void *runner(void *param); 
int main(void)
{
	static int i, scope,NUM_THREADS,num_thread;
	num_thread=0;
	printf("Enter the number of threads: ");
	scanf("%d",&NUM_THREADS);
	mythread threads[NUM_THREADS];
	pthread_t tid[NUM_THREADS]; 
	pthread_attr_t attr; 		

	pthread_attr_init(&attr);
	if (0 != pthread_attr_getscope(&attr,&scope)){
		fprintf(stderr, "Unable to get scheduling scope.\n");
	}

	else {
		num_thread++;
		if (scope == PTHREAD_SCOPE_PROCESS)
		{
			printf("PTHREAD_SCOPE_PROCESS\n");
		}

		else if (scope == PTHREAD_SCOPE_SYSTEM)
		{
			printf("PTHREAD_SCOPE_SYSTEM\n");
		}
		else 
		{
			fprintf(stderr,"Illegal scope value.\n");
		}

	}
	if (0 != pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM))
		printf("Unable to set scheduling policy.\n");

	for (i = 0; i < NUM_THREADS; i++){
		threads[i].ID=rand()%(NUM_THREADS*10);
		threads[i].iteration=i;
		pthread_create(&tid[i],&attr,runner,(void*)&threads[i]); 
		//pthread_create(&tid[i],&attr,runner,NULL);//en clase
		printf("I am thread %d. Created new thread (%ld) in iteration %ld...\n",num_thread,threads[i].ID,threads[i].iteration);
		
		if (0 == (i+1)%5){
			 sleep(1);
		}
	}
	for (i = 0; i < NUM_THREADS; i++){
		pthread_join(tid[i], NULL);
	}

	return 0;
}

void *runner(void *param) 
{
	mythread *tid=param;
	printf("Hello from thread %ld - I was created in iteration %ld\n",tid->ID,tid->iteration);
	
	pthread_exit(0);
}
