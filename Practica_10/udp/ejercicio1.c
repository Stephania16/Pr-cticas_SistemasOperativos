#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 80

/*ESQUEMA SERVIDOR UDP:
	- socket()
	- bind()
	- recvfrom()
	- sendto()
	- close()
*/

/*SERVIDOR
  ./servidor_udp ::1 3000
   nc -u -6 ::1 3000
*/
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
	
	if(argc < 2){ perror("Error de argumentos.\n");}

	/*Devuelve una lista de estructuras de direcciones*/
	getaddrinfo(argv[1],argv[2], &hints, &res);

	/*crea un extremo de comunicación*/
	udp_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	/*Define la dirección en la que se escuchará. Enlaza un nombre a un conector.*/
	bind(udp_socket, (struct sockaddr *) res->ai_addr, res->ai_addrlen);
	
	while(1){
		struct sockaddr_storage cliente;
		socklen_t cliente_len = sizeof(cliente);
		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];

		/*Recibe mensajes*/
		printf("Recibiendo mensajes.\n");
		int recv = recvfrom(udp_socket, buffer, MAX_SIZE, 0, (struct sockaddr *) &cliente, &cliente_len);
		
		buffer[recv] = '\0';
		/*Enviamos la hora*/
		if(strcmp(buffer, "t") == 0 || strcmp(buffer, "t\n") == 0)
		{
			time_t t = time(NULL);
			struct tm *lc;
			lc = localtime(&t);
			strftime(buffer,50,"%H:%M:%S %P",lc);
			sendto(udp_socket, buffer, strlen(buffer), 0, (struct sockaddr *) &cliente, cliente_len);
		}
		/*Enviamos la fecha*/
		else if(strcmp(buffer, "d") == 0 || strcmp(buffer, "d\n") == 0)		
		{
			time_t t = time(NULL);
			struct tm *lc;
			lc = localtime(&t);
			strftime(buffer,50,"Hoy es %A",lc);
			sendto(udp_socket, buffer, strlen(buffer), 0, (struct sockaddr *) &cliente, cliente_len);

		}
		/*Cerramos conexión*/
		else if(strcmp(buffer, "q") == 0 || strcmp(buffer, "q\n") == 0)		
		{
			sendto(udp_socket, "CERRANDO CONEXION..", 20, 0, (struct sockaddr *) &cliente, cliente_len);
			freeaddrinfo(res);
			close(udp_socket);
			_exit(1);
		}
		
		/*Obtener host y service*/
		getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

		printf("Host: %s, Puerto: %s\n", host, serv);
		printf("Mensaje (%i): %s\n", recv, buffer);
		sleep(5);
	}
	freeaddrinfo(res);
	close(udp_socket);
	
	return 0;
}
