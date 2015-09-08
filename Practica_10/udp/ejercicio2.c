#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

/*ESQUEMA CLIENTE UDP:
	- socket()
	- sendto()
	- recvfrom()
	- close()
*/

#define MAX_SIZE 80

/*CLIENTE*/
int main(int argc, char** argv){
	int udp_socket;
	struct addrinfo hints;
	struct addrinfo *res;
	char buffer[MAX_SIZE];

	/*struct addrinfo*/
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;/*IPV4 O IPV6*/
	hints.ai_socktype = SOCK_DGRAM; /*UDP*/
	hints.ai_protocol = 0;

	if(argc < 3){ perror("Error de argumentos.\n");}	

	/*Devuelve una lista de estructuras de direcciones*/
	getaddrinfo(argv[1],argv[2], &hints, &res);

	/*crea un extremo de comunicación*/
	udp_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	
	struct sockaddr_storage cliente;
	socklen_t cliente_len = sizeof(cliente);

	sendto(udp_socket, argv[3], strlen(argv[3]), 0, (struct sockaddr *) res->ai_addr, res->ai_addrlen);

	/*Recibe mensajes*/
	int recv = recvfrom(udp_socket, buffer, MAX_SIZE, 0, (struct sockaddr *) &cliente, &cliente_len);
		
	buffer[recv] = '\0';
	
	printf("Mensaje recibido: %s\n", buffer);
	freeaddrinfo(res);
	close(udp_socket);
	
	return 0;
}
