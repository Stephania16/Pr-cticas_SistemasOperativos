#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>


/*ESQUEMA SERVIDOR TCP:
	- socket()
	- bind()
	- listen()
	- accept()
	- recv()
	- send()
	- close()
*/
#define MAX_SIZE 80
int main(int argc, char** argv){

	int sd, info;
	struct addrinfo hints;
	struct addrinfo *res;
	char buffer[MAX_SIZE];
	int sdC = 0;
	int r = 0;
	int i;
	int uid = 0, gid = 0, ino = 0;
	int estado = 0;

	/*struct addrinfo*/
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;/*IPV4 O IPV6*/
	hints.ai_socktype = SOCK_STREAM; /*TCP*/
	hints.ai_protocol = 0;

	if(argc < 2){ perror("Error de argumentos.\n");}

	/*Devuelve una lista de estructuras de direcciones*/
	getaddrinfo("::1",argv[1], &hints, &res);

	printf("Creando socket.\n");
	/*crea un extremo de comunicación*/
	sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	/*Define la dirección en la que se escuchará. Enlaza un nombre a un conector.*/
	printf("Bind.\n");
	bind(sd, (struct sockaddr *) res->ai_addr, res->ai_addrlen);
	
	struct sockaddr_storage cliente;
	socklen_t cliente_len = sizeof(cliente);
	char host[NI_MAXHOST];
	char serv[NI_MAXSERV];
	//struct stat sb;

	for(;;){
		
		printf("Listen.\n");		
		if(listen(sd,10)){ return -1; }
		printf("Accept.\n");
		sdC = accept(sd, (struct sockaddr *) &cliente, &cliente_len);
		int pid;
		pid = fork();
		int num_pid[2];
		if(pid == -1){ perror("Error de fork.\n");}
		else if(pid == 0){ /*Hijo*/	
			close(sd);
			for(;;){
				close(sd);
				printf("Recibiendo.\n");
				r = recv(sdC, buffer, MAX_SIZE, 0);
				buffer[r]= '\0';

				/*Obtener host y service*/
				getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
				printf("Host: %s, Puerto: %s\n", host, serv);

				if(buffer[0] == 'r') /*Comprobamos que el primer elemento del buffer sea i*/	
				{	
					char ruta[MAX_SIZE];
					/*Pasamos ./archivo.c como string a ruta*/
					sscanf(&buffer[2],"%s", ruta);
					unlink(ruta);
					send(sdC, "Eliminado archivo\n", 18, 0);
						
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
		close(sdC);
		/*i = 0;
		int status;
		while(i < 2){
			wait(&status);
			//printf("Fin proceso %i, código de salida %i\n", num_pid[i], WEXITSTATUS(status));
			i++;
		}*/
	}

	freeaddrinfo(res);
	

	return 0;
}
