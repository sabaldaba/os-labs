/* 
 * File:   main.c
 * Author: saldana
 *
 * Created on 11 de septiembre de 2018, 11:48 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_PER_LINE 10
#define TOTAL_SIZE 4000000
#define COLUMN_SIZE 20000
#define ROW_SIZE 2000
#define PATH_A "/home/saldana/os-labs/lab1.9/matA.dat"
#define PATH_B "/home/saldana/os-labs/lab1.9/matB.dat"

/*Function that receives as parameter a String which is the path of the
 * file that you intend to read. See the README file to see the format that the file
 * should contains
 */

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
		temp = strtoul(buff, &stopstring, 10);
		matrix[i] = temp;
	}
	fclose(fp);
	return matrix;
}

long *getRow(int row, long *matrix)
{
	long *returnMatrix = malloc(ROW_SIZE*sizeof(long));
	const unsigned int startPos = ROW_SIZE * row;	/* Get initial position of row user is requesting*/
	for(int i=0; i<ROW_SIZE;i++)
	{
		returnMatrix[i] = matrix[startPos+i];
		printf("%ld\n", returnMatrix[i]);	/* Just to see if its actually working (Y)*/
	}
	return returnMatrix;
}

long *getColumn(int column, long *matrix)
{
	long *returnMatrix = malloc(COLUMN_SIZE*sizeof(long));
	const unsigned int startPos = COLUMN_SIZE * column;	/* Get initial position of column user is requesting*/	
	
}

int main(int argc, char **argv)
{
	uint8_t *NUM_BUFFERS;
	long *matrixA, *matrixB;
	NUM_BUFFERS = argv[1];
	unsigned long *ptr;
	printf("Number of buffers wanted: %s\n", NUM_BUFFERS);

	matrixA = readMatrix(PATH_A);
	matrixB = readMatrix(PATH_B);
	getRow(0, matrixA);
		
	
	return 0;
}


