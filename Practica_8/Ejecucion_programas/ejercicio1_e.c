#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
	int salida = 0;
	printf("Ejemplo exec\n");
	execl("/bin/ls", "ls", "-1", NULL);
	printf("Salida: %d\n", salida);
	exit(salida);
	return 0;
}
