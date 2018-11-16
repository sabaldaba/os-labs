/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct{
	char operation;
	int values[10];
	int size;
}package; 

int main(int argc, char *argv[])
{
    int sockfd;
    int len;
    struct sockaddr_un address;
    int result;
    char ch = 'A';

/*  Create a socket for the client.  */

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

/*  Name the socket, as agreed with the server.  */

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, "server_socket");
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    package p;

    if(result == -1) {
        perror("oops: client1");
        exit(1);
    }
	int i;
	p.operation = *argv[1];
	p.size=argc-2;
	for(i=2;i<argc;i++){
		p.values[i-2]=atoi(argv[i]);
	}

/*  We can now read/write via sockfd.  */

    write(sockfd, &p, sizeof(package));
	long res=0;
    read(sockfd, &res, sizeof(long));
	if(res==-99999){
		 printf("Operation unsupported, please contact your administrator\n");
	}
	else{
	    printf("Client meesage: server result = %ld\n", res);
	}
    close(sockfd);
    exit(0);
}

