/* 
 * File:   multiplier.c
 * Author: saldana
 *
 * Created on 11 de septiembre de 2018, 11:48 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_PER_LINE 10
#define TOTAL_SIZE 4000000
#define COLUMN_SIZE 2000
#define ROW_SIZE 2000
#define PATH_A "matA.dat"
#define PATH_B "matB.dat"
#define NUM_THREADS 2000

/*Strcture of a Thread
 */
typedef struct
{
	long ID;
	long iteration;	
	long fila;
} Thread;

/*Function that receives as parameter a String which is the path of the
 * file that you intend to read. See the README file to see the format that the file
 * should contain
 */

int NUM_BUFFERS;
long **buffers; //array the buffers para guardar operaciones
long *result; //matriz con valor final
pthread_mutex_t *mutexes; // Mutexes that will help to know which buffer is available
long *globalB;
long *globalA;

long *readMatrix(char filename [])
{
	FILE *fp;
	unsigned char buff[MAX_PER_LINE];
	long *matrix = malloc(TOTAL_SIZE*sizeof(long));	/* Create array in heap so we can actually return it*/
	fp = fopen(filename, "r");		/* Open file with filename path*/	
	char *stopstring;
	unsigned long temp; 		/* Variable temporal para almacenar buff en formato UL*/
	for(int i=0;i<TOTAL_SIZE;i++)		/* Traverse through .txt and assigned what is found to array matrix[]*/
	{
        	fscanf(fp, "%s", buff); /* Can be reduced but for now it works*/
		temp = strtoul(buff, &stopstring, MAX_PER_LINE);
		matrix[i] = temp;
		//printf("Valor: %ld\n", temp);
	}
	fclose(fp);
	return matrix;
}

long *getRow(const int row, long *matrix)
{
	long *returnMatrix = malloc(ROW_SIZE*sizeof(long));
	const unsigned int startPos = ROW_SIZE * row;	/* Get initial position of row user is requesting*/
	for(unsigned int i=0; i<ROW_SIZE;i++)
	{
		returnMatrix[i] = matrix[startPos+i];
		//printf("%ld\n", returnMatrix[i]);	/* Just to see if its actually working (Y)*/
		//printf("fila %d\n", row);	/* Just to see if its actually working (Y)*/
	}
	return returnMatrix;
}

long *getColumn(const int column, long *matrix)
{
	long *returnMatrix = malloc(COLUMN_SIZE*sizeof(long));
	const unsigned int startPos = COLUMN_SIZE * column;	/* Get initial position of column user is requesting*/	
	for(unsigned int i=0; i<ROW_SIZE; i++)
	{
		returnMatrix[i] = matrix[(ROW_SIZE*i)+column];
		//printf("%d - %ld\n",i, returnMatrix[i]);	/* Just to see if its actually working (Y)*/
		//printf("Columna %d\n", column);
	}
	return returnMatrix;
}


int getLock() // Search for an available buffer, if so it returns the available lock id which is the same 
{
	int returnVal;	
	unsigned char set;
	returnVal = -1;
	set=0;
	for(int i=0; i<NUM_BUFFERS, 0==set; i++)
	{
		if(0==pthread_mutex_trylock(&mutexes[i]))
		{
			returnVal=i;
			set++;
		}
	}
	return returnVal;
}

int releaseLock(int lock) // Releases a buffer and unlock the mutex. Returns 0 for successful unlock, otherwise -1.
{
	return pthread_mutex_unlock(&mutexes[lock]);
}

void destroy_mutex()
{
	for(int i=0; i<NUM_BUFFERS; i++)
	{	
		pthread_mutex_destroy(&mutexes[i]);
	}
}

void freeBuffers()
{
	for(int i=0; i<NUM_BUFFERS; i++)
	{
		free(buffers[i]);
	}
	free(buffers);
}

long dotproduct(long *vec1, long *vec2)
{
	long returnVal;
	returnVal=0;
//	for(int i=0; i<6; i++)
	for(int i=0; i<ROW_SIZE; i++)
	{
		returnVal=(vec1[i]*vec2[i]+returnVal);
//		printf("Return: %ld\n", returnVal);
	}
	return returnVal;
}

/*-----------------Function to free all global arrays allocated dynamically--------------------------*/
void freeALL()
{
	destroy_mutex();
	freeBuffers();
}

void *runner(void *param) 
{
	Thread *tid=param;
	//printf("Hello from thread %ld - I was created in iteration %ld with file: %ld\n",tid->ID,tid->iteration, tid->fila);
	int buffer; //available buffer
	buffer = getLock();
	/*while(-1 == buffer)
	{
		buffer = getLock();
		sleep(1);
	}*/
	buffers[0][0] = dotproduct(getRow(0,globalA), getColumn(0, globalB));
/*	for(int i=0;i<COLUMN_SIZE;i++) //for para columnas de B
	{
		for(int j=0;j<ROW_SIZE; j++) //for columnas de fila pasada
		{
			buffers[buffer][j] = dotproduct(getRow(i,globalA), getColumn(j, globalB));
			//printf("Columna: %d\n", j);
			getColumn(j, globalB);
		}
	}*/
	releaseLock(buffer);
	pthread_exit(0);
}

long *multiply(long *matA, long *matB)
{
	/*Threads variables*/
	static int i, scope,num_thread;
	Thread threads[NUM_THREADS]; /* Array of 2000 threads*/
	pthread_t tid[NUM_THREADS]; /* Know squat about it's purpose*/
	pthread_attr_t attr; /* Same ^*/
	num_thread = 0; /* Dont know why is 0, Guess is just number arbitrary, wont need at the end i think*/
	pthread_attr_init(&attr); /* Again know squat*/
/************************************************+* THREADS CODE********************************************************/
	if (0 != pthread_attr_getscope(&attr,&scope)){
		fprintf(stderr, "Unable to get scheduling scope.\n");
	}

	if (0 != pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM))
	{
		printf("Unable to set scheduling policy.\n");
	}

	for (i = 0; i < NUM_THREADS; i++){
		threads[i].ID=rand()%(NUM_THREADS*10);
		threads[i].iteration=i;
		threads[i].fila = i;
		pthread_create(&tid[i],&attr,runner,(void*)&threads[i]); 
		//printf("I am thread %d. Created new thread (%ld) in iteration %ld...\n",num_thread,threads[i].ID,threads[i].iteration);
		/*if (0 == (i+1)%5){
			 sleep(1);
		}*/
	}

	for (i = 0; i < NUM_THREADS; i++){
		pthread_join(tid[i], NULL);
	}
	return 0;
}

int main(int argc, char **argv)
{
	/* Variables for matrixA and matrixB retrieval*/
//	uint8_t *NUM_BUFFERS; moved to global
	long *matrixA, *matrixB;
	char *stopstring;
	NUM_BUFFERS = strtol(argv[1], &stopstring, 10);

//	long **buffers; //array the buffers para guardar operaciones moved to global
	buffers = (long**)malloc(ROW_SIZE*(sizeof(long*))); //allocate memory for double array

	/* For used to create double array of buffers*/
	for(int i=0; i<ROW_SIZE;i++)
	{
		*(buffers+i)=(long*)malloc(ROW_SIZE*sizeof(long));
	}

	/************************************************** Programm **************************************************/

	printf("Number of buffers wanted: %i\n", NUM_BUFFERS); //Just to try
//	matrixA = readMatrix(PATH_A);
//	matrixB = readMatrix(PATH_B);
	globalB = readMatrix(PATH_B);
	globalA = readMatrix(PATH_A);
	/********************************************************MUTEX*************************************/
	mutexes = malloc((long)NUM_BUFFERS*sizeof(pthread_mutex_t)); 
//	for(int i=0; i<(int)NUM_BUFFERS;i++)
	for(int i=0; i<1;i++)
	{
		 if (pthread_mutex_init(&mutexes[i], NULL) != 0)
    		{
	        printf("\n Mutex init failed\n");
        	return 1;
		}
	}
	
	multiply(matrixA, matrixB);
	/*******------------------HOW TO USE MUTEX--------------**********/
		
	/*printf("Val: %d\n", pthread_mutex_trylock (&mutexes[0]));
	printf("Val: %d\n", pthread_mutex_trylock (&mutexes[0]));
	pthread_mutex_unlock (&mutexes[0]);
	printf("Val: %d\n", pthread_mutex_trylock (&mutexes[0]));
	printf("Hola\n");
	/*--------------------------------END MUTEX TRAINING-----------------------*/
	long *x;
	x = malloc(9*sizeof(long));
	for(int i=0;i<9;i++){
		x[i]=i;
		//printf("valor de x: %d\n", i);
	}
	free(x);
	free(matrixA);
	free(matrixB);
	freeALL();
	return 0;
}


