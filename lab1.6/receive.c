/*#include <stdio.h>
#include <stdlib.h>

int system(const char *command);

int main(int argc, char *argv[])
{
	system("gedit"&"simple");
	int i;
	printf("The following arguments were passed to main(): ");
	for(i=1; i<argc; i++){
		//printf("%s ", argv[i]);
		//system(argv[i]);
	}
	printf("\n");
	
	
} */
#include<stdio.h>
#include<stdlib.h>
/*int main()
{
	char str[256], buf[256];
	printf("myshell-> ");
	scanf("%s",str);
	sprintf(buf, "/bin/sh -c %s", str);
	system(buf);
	return 0;
}*/
#include <stdio.h>
#include <string.h>
#define MAX_SIZE_BUFF 256
/*int main () {
	char command[MAX_SIZE_BUFF];
	while(1){
	printf("myshell-> ");
//	scanf("%s",command);
	fgets(command, MAX_SIZE_BUFF, stdin);
//	strcpy( command, "gedit simple.c" );
	system(command);
	
	}

   return(0);
} */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_SIZE_BUFF 256
#define MAX_SIZE_HIST 16
#define TRUE 10

unsigned char command[MAX_SIZE_BUFF];	
unsigned char *name[]={"/bin/bash","-c","gedit dd.c", NULL};	
unsigned char *history[]={"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"};
unsigned char i=0;
char * ( *ptr )[50] = &history;
int main(){	
	pid_t pid;
	while(1){
		printf("myshell-> ");
		fflush(stdout);  
		fgets(command, MAX_SIZE_BUFF, stdin);
		( *ptr )[i] = command;
		i++;
		name[2] = command;
		print();
		pid = fork();
		if (pid < 0) { /* error occurred */
			fprintf(stderr, "Fork Failed\n");
			return 1;
		}
		if (pid == 0) { /* child process */
			if(strcmp(command, "history") == TRUE){
				print();
//				name[2] = history[(i)%16];		
			}
			execvp(name[0], name);
		}
		else if (pid > 0) { /* parent process */
			//if(STOP_FLAG){
				wait(NULL);
			//}
			
		}
	}
}

void print(){
	for(int i=0;i<15;i++){	
		if(history[i]!=NULL){
			printf("%s\n",history[i]);				
		}

	}		
}





