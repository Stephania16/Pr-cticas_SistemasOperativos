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

	struct sched_param schParam;
	schParam.sched_priority = sched_get_priority_max(SCHED_RR);
	sched_setscheduler(0, SCHED_RR, &schParam);

	setpriority (PRIO_PROCESS, 0, 12);

	fdOut = open("/tmp/daemon.out", O_RDWR);
	fdErr = open("/tmp/daemon.err",O_RDWR);
	fdIn = open("/tmp/null",O_RDWR);

	dup2(fdIn,STDIN_FILENO);
	dup2(fdOut,STDOUT_FILENO);
	dup2(fdErr,STDERR_FILENO);

	execvp (argv[1], onlyArgs);

	close(fdErr);
	close(fdOut);
	close(fdIn);

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
