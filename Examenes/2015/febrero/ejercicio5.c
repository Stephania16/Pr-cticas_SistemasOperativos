#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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
	int udp_socket, info;
	struct addrinfo hints;
	struct addrinfo *res;
	char buffer[MAX_SIZE];
	int uid = 0, gid = 0, ino = 0;

	/*struct addrinfo*/
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;/*IPV4 O IPV6*/
	hints.ai_socktype = SOCK_DGRAM; /*UDP*/
	hints.ai_protocol = 0;
	
	if(argc < 2){ perror("Error de argumentos.\n");}

	/*Devuelve una lista de estructuras de direcciones*/
	info = getaddrinfo("::",argv[1], &hints, &res);

	if(info < 0) return -EINVAL;

	/*crea un extremo de comunicación*/
	udp_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	/*Define la dirección en la que se escuchará. Enlaza un nombre a un conector.*/
	bind(udp_socket, (struct sockaddr *) res->ai_addr, res->ai_addrlen);
	pid_t pid;
	int i;
	for(i = 0; i < 2; i++)
	{	
		pid = fork();
		if(pid == -1) { perror("Error fork.\n"); }
		else if(pid == 0){		
			while(1){
				struct sockaddr_storage cliente;
				socklen_t cliente_len = sizeof(cliente);
				char host[NI_MAXHOST];
				char serv[NI_MAXSERV];

				/*Recibe mensajes*/
				int recv = recvfrom(udp_socket, buffer, MAX_SIZE, 0, (struct sockaddr *) &cliente, &cliente_len);
		
				buffer[recv] = '\0';

				/*Obtener host y service*/
				getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
				printf("Host: %s, Puerto: %s\n", host, serv);

				/*Enviamos la hora*/
				if(buffer[0] == 'i')		
				{	
					char ruta[MAX_SIZE];
					char msg[MAX_SIZE];
					sscanf(&buffer[2],"%s", ruta);
					struct stat sb;
					if(stat(ruta, &sb) == -1){
						perror("Error de stat.\n");
					}
					else{
						uid = sb.st_uid;
						gid = sb.st_gid;
						ino = sb.st_ino;
						sprintf(msg, "UID: %i, GID: %i, INO: %i\n", uid, gid, ino);
						sendto(udp_socket, msg, strlen(msg), 0, (struct sockaddr *) &cliente, cliente_len);
					}

				}
				/*Cerramos conexión*/
				else if(buffer[0] == 'q')		
				{
					sendto(udp_socket, "CERRANDO CONEXION..\n", 20, 0, (struct sockaddr *) &cliente, cliente_len);
					freeaddrinfo(res);
					close(udp_socket);
					return 0;
				}
			}
			close(udp_socket);
			_exit(0);
		}
		
	}
	i = 0;
	int status;
	while(i < 2){
		wait(&status);
		i++;
	}
	freeaddrinfo(res);
	close(udp_socket);
	
	return 0;
}
