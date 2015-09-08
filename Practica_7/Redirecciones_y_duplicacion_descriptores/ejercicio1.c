/*dup y dup2*/
#include <unistd.h>
/*open*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/*printf*/
#include <stdio.h>


/*https://baulderasec.wordpress.com/programacion/programacion-con-linux/3-trabajando-con-los-archivos/acceso-de-bajo-nivel-a-archivos/dup-y-dup2/*/

int main(int argc, char **argv){
	int fd;
	fd=open(argv[1], O_CREAT | O_RDWR, 0640);
	/*Se cambia la salida estándar al fichero abierto */
	dup2(fd, 1);
	
	close(fd);

	printf("HOLA MUNDO\n");
	dup(1);
	return 0;
}
