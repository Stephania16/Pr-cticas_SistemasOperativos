#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define MAX_SIZE 80
/*ESQUEMA SERVIDOR UDP:
	- socket()
	- bind()
	- recvfrom()
	- sendto()
	- close()
*/

/*SERVIDOR*/
int main(int argc, char** argv){
	int udp_socket;
	struct addrinfo hints;
	struct addrinfo *res;
	char buffer[MAX_SIZE];
	fd_set set;
	int recv = 0;
	struct timeval t;
	t.tv_sec = 2;
	t.tv_usec = 0;

	/*struct addrinfo*/
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;/*IPV4 O IPV6*/
	hints.ai_socktype = SOCK_DGRAM; /*UDP*/
	hints.ai_protocol = 0;
	
	printf("argc: %d\n", argc);
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
		
		FD_ZERO(&set);
		FD_SET(udp_socket, &set);
		FD_SET(0, &set);/*entrada estandar*/
		
		select(udp_socket + 1, &set, NULL, NULL, &t);
		if(FD_ISSET(0, &set)){ /*entrada estandar*/
			scanf("%s", buffer);
		}
		else if(FD_ISSET(udp_socket, &set))
		{
			/*Recibe mensajes*/
			printf("Recibiendo mensajes.\n");
			recv = recvfrom(udp_socket, buffer, MAX_SIZE, 0, (struct sockaddr *) &cliente, &cliente_len);
			buffer[recv] = '\0';
		}		

		/*Enviamos la hora*/
		if(strcmp(buffer, "t") == 0 || strcmp(buffer, "t\n") == 0)		{
			time_t t = time(NULL);
			if(FD_ISSET(0, &set)){ 
				printf("%s\n", ctime(&t));
			}
			else if(FD_ISSET(udp_socket, &set))
			{	
				sendto(udp_socket, ctime(&t), strlen(ctime(&t)), 0, (struct sockaddr *) &cliente, cliente_len);
			}
		}
		/*Enviamos la fecha*/
		else if(strcmp(buffer, "d") == 0 || strcmp(buffer, "d\n") == 0)		
		{
			time_t t = time(NULL);
			struct tm *lc;
			lc = localtime(&t);
			strftime(buffer, 50,"Hoy es %A",lc);
			if(FD_ISSET(0, &set)){ 
				printf("%s\n", buffer);
			}
			else if(FD_ISSET(udp_socket, &set))
			{
				sendto(udp_socket, buffer, strlen(buffer), 0, (struct sockaddr *) &cliente, cliente_len);
			}

		}
		/*Cerramos conexión*/
		else if(strcmp(buffer, "q") == 0 || strcmp(buffer, "q\n") == 0)		{
			if(FD_ISSET(0, &set)){ 
				printf("Salir.\n");
			}
			else if(FD_ISSET(udp_socket, &set))
			{
				sendto(udp_socket, "CERRANDO CONEXION..", 20, 0, (struct sockaddr *) &cliente, cliente_len);
			}
			freeaddrinfo(res);
			close(udp_socket);
			_exit(1);
		}
		
		if(recv > 0){
			/*Obtener host y service*/
			getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

			printf("Host: %s, Puerto: %s\n", host, serv);
			printf("Mensaje (%i): %s\n", recv, buffer);
			recv = 0;
			sleep(3);
		}
	}
	freeaddrinfo(res);
	close(udp_socket);
	
	return 0;
}
