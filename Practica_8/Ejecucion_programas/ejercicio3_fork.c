#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


//Prototipos
void ejercicio2(void);

int main(int argc, char** argv){
	int fdOut, fdErr, fdIn;
	
	char **onlyArgs = argv + 1;
	
	pid_t pid = fork();

	if(pid == -1) perror("Error fork");
	else if(pid == 0){

		fdOut = open("/tmp/daemon.out", O_RDWR | O_CREAT);
		fdErr = open("/tmp/daemon.err",O_RDWR | O_CREAT);
		fdIn = open("/tmp/null",O_RDWR | O_CREAT);

		dup2(fdIn,STDIN_FILENO);
		dup2(fdOut,STDOUT_FILENO);
		dup2(fdErr,STDERR_FILENO);

		execvp (argv[1], onlyArgs);

		close(fdErr);
		close(fdOut);
		close(fdIn);
	}
	else{
		int status;
		wait(&status);
		printf("El comando termino de ejecutarse\n");
	}
	return 0;
}




/*
Hacer el ejecutable con:

$> gcc -o demonio demonio.c

Crear los archivos en /tmp con:

$> cd /tmp
$> touch daemon.err daemon.out null

Y ahora probamos el programa:

-------Salida estandar --------

$> ./demonio ls -laht

Para ver que realmente lo ha hecho bien hacemos

$> cat /tmp/daemon.out

------Salida de error ---------
$> ./demonio ls -2

Para ver que realmente lo ha hecho bien hacemos

$> cat /tmp/daemon.err

-------Salida null -------

*/
