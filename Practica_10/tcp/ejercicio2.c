#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*ESQUEMA SERVIDOR TCP:
	- socket()
	- connect()
	- send()
	- recv()
	- close()
*/
#define MAX_SIZE 80
int main(int argc, char** argv){

	int sd;
	struct addrinfo hints;
	struct addrinfo *res;
	char buffer[MAX_SIZE];
	int sdC = 0;
	int r = 0;

	/*struct addrinfo*/
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;/*IPV4 O IPV6*/
	hints.ai_socktype = SOCK_STREAM; /*TCP*/
	hints.ai_protocol = 0;

	if(argc < 3){ perror("Error de argumentos.\n");}

	/*Devuelve una lista de estructuras de direcciones*/
	getaddrinfo(argv[1],argv[2], &hints, &res);

	printf("Creando socket.\n");
	/*crea un extremo de comunicación*/
	sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	printf("Connect.\n");
	connect(sd, res->ai_addr, res->ai_addrlen);	

	while(strcmp("Q\n", buffer) != 0){
		scanf("%s", buffer);
		if (strcmp("Q\n", buffer) == 0 ) {
			close(sd);
			_exit(1);
		}
		send(sd, buffer, strlen(buffer), 0);
		printf("Recibiendo.\n");
		r = recv(sd, buffer, MAX_SIZE, 0);
		buffer[r]= '\0';
		printf("Mensaje: %s\n", buffer);
	}
	freeaddrinfo(res);
	close(sd);

	return 0;
}
