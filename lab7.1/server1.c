/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#define TRUE 1
typedef struct{
	char operation;
	int values[10];
	int size;
}package; 

int add(int valores[],int size){
	int i;
	int temp=0;
	for(i=0;i<size;i++){
		temp+=valores[i];
	}
	return temp;
}
int subs(int valores[],int size){
	int i;
	int temp=0;
	for(i=0;i<size;i++){
		temp-=valores[i];
	}
	return temp;
}
long mult(int valores[],int size){
	int i;
	long temp=1;
	for(i=0;i<size;i++){
		temp=temp*valores[i];
	}
	return temp;
}

int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;

/*  Remove any old socket and create an unnamed socket for the server.  */

    unlink("server_socket");
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

/*  Name the socket.  */

    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, "server_socket");
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

/*  Create a connection queue and wait for clients.  */

    listen(server_sockfd, 5);
    while(TRUE) {
        char ch;

        printf("server waiting\n");

/*  Accept a connection.  */

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, 
            (struct sockaddr *)&client_address, &client_len);

/*  We can now read/write to client on client_sockfd.  */
	package p;
        read(client_sockfd, &p, sizeof(package));

	int i;
	long resu=0;
	if(p.operation=='+'){
		resu=add(p.values,p.size);
	}
	else if(p.operation=='-'){
		resu=subs(p.values,p.size);
	}
	else if(p.operation=='x'){
		resu=mult(p.values,p.size);
	}
	else{
		resu=-99999;
	}

	printf("Server result: %ld \n",resu);
        ch++;
        write(client_sockfd, &resu, sizeof(long));
        close(client_sockfd);
    }
}
