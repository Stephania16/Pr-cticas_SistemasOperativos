/*uname*/
#include <sys/utsname.h>
/*perror y printf*/
#include <errno.h>
#include <stdio.h>
/*sysconf y pathconf*/
#include <unistd.h>

int main(){

	int valor_uname;
	struct utsname buffer;
	/*Ejercicio 2*/ 	
	valor_uname = uname(&buffer);
	if(valor_uname == -1)
	{
		perror("ERROR UNAME");
		printf("Número del error %i\n", errno);	
	}

	/*Información de cada aspecto del sistema*/
	printf("INFORMACIÓN DEL SISTEMA\n");
	printf("Nombre del sistema: %s\n", buffer.sysname);
	printf("Nombre de la máquina: %s\n", buffer.nodename);
	printf("Actualización: %s\n", buffer.release);
	printf("Versión: %s\n", buffer.version);
	printf("Nombre de la arquitectura: %s\n\n", buffer.machine);

	/*Ejercicio 3*/
	printf("INFORMACIÓN DE CONFIGURACIÓN DEL SISTEMA\n");
	printf("Longitud máxima de los argumentos: %ld\n", sysconf(_SC_ARG_MAX)); 
	printf("Número máximo de hijos: %ld\n", sysconf(_SC_CHILD_MAX));
	printf("Número máximo de ficheros: %ld\n\n", sysconf(_SC_OPEN_MAX));

	/*Ejercicio 4*/
	printf("CONFIGURACIÓN DEL SISTEMA DE FICHEROS\n");
	printf("Número máximo de enlaces: %ld\n", pathconf("/",_PC_LINK_MAX));
	printf("Tamaño máximo de una ruta: %ld\n", pathconf("/",_PC_PATH_MAX));
	printf("Nombre de fichero: %ld\n\n", pathconf("/",_PC_NAME_MAX));

	return 0;
}






