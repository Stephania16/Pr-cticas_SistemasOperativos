/*open*/
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
/*printf*/
#include <stdio.h>
/*error*/
#include <errno.h>

int main(){


	int fichero;
	fichero = open("./ej_fichero", O_CREAT ,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IXOTH);
	if(fichero == -1)
	{  return -EACCES; }
	
	printf("Fichero creado con éxito.\n");
	return 0;
}
