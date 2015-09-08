#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*ESQUEMA SERVIDOR TCP:
	- socket()
	- bind()
	- listen()
	- accept()
	- recv()
	- send()
	- close()
*/

/*./servidor 2222
   nc ::1 2222
*/
#define MAX_SIZE 80
int main(int argc, char** argv){

	int sd, info;
	struct addrinfo hints;
	struct addrinfo *res;
	char buffer[MAX_SIZE];
	int sdC = 0;
	int r = 0;
	int uid = 0, gid = 0, ino = 0;

	/*struct addrinfo*/
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;/*IPV4 O IPV6*/
	hints.ai_socktype = SOCK_STREAM; /*TCP*/
	hints.ai_protocol = 0;

	if(argc < 2){ perror("Error de argumentos.\n");}

	/*Devuelve una lista de estructuras de direcciones*/
	getaddrinfo("0.0.0.0",argv[1], &hints, &res);


	printf("Creando socket.\n");
	/*crea un extremo de comunicación*/
	sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	/*Define la dirección en la que se escuchará. Enlaza un nombre a un conector.*/
	printf("Bind\n");
	bind(sd, (struct sockaddr *) res->ai_addr, res->ai_addrlen);

	struct sockaddr_storage cliente;
	socklen_t cliente_len = sizeof(cliente);
	char host[NI_MAXHOST];
	char serv[NI_MAXSERV];
/*	printf("Listen\n");		
	if(listen(sd,10)){ return -1; }
	printf("Accept\n");
	sdC = accept(sd, (struct sockaddr *) &cliente, &cliente_len);*/
	struct stat sb;	
	int status;
	int i = 0;
	int pid;
	int num_pid[MAX_SIZE];
	for(;;){

		printf("Listen\n");		
		if(listen(sd,10)){ return -1; }
		printf("Accept\n");
		sdC = accept(sd, (struct sockaddr *) &cliente, &cliente_len);
		pid = fork();
		if(pid == -1){ perror("Error de fork.\n");}
		else if(pid == 0){ /*Hijo*/	
			close(sd);
			for(;;){
					printf("Recibiendo.\n");
					r = recv(sdC, buffer, MAX_SIZE, 0);
					buffer[r]= '\0';
					/*Obtener host y service*/
					getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

					printf("[%d] Host: %s,  Puerto: %s\n", getpid(), host, serv);
		
					if(buffer[0] == 'i') /*Comprobamos que el primer elemento del buffer sea i*/	
					{	
						char ruta[MAX_SIZE];
						char msg[MAX_SIZE];
						/*Pasamos ./archivo.c como string a ruta*/
						sscanf(&buffer[2],"%s", ruta);
		
						/*En stat pasamos ruta*/
						if(stat(ruta, &sb) == -1){
							int errno;
							char *st = strerror(errno);
							send(sdC, st, strlen(st), 0);
						}
						else{
							uid = sb.st_uid;
							gid = sb.st_gid;
							ino = sb.st_ino;
							sprintf(msg, "[PID: %d] UID: %i, GID: %i, INO: %i\n", getpid(), uid, gid, ino);
							send(sdC, msg, strlen(msg), 0);
						}

			
					}
					/*Cerramos conexión*/
					else if(buffer[0] == 'q')	/*Comprobamos que el primer elemento del buffer sea q*/	
					{
						send(sdC, "CERRANDO CONEXION..\n", 20, 0);
						freeaddrinfo(res);
						close(sd);
						return 0;
					}
	
			}
		}
		
			
		}
		
		close(sdC);
	
		
	}
	freeaddrinfo(res);
	return 0;
}
