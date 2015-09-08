#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

int main(int argc, char **argv){

	int fd;
	struct flock fl;
	fd = open(argv[1], O_CREAT | O_RDWR, 0777);
	if(fd == -1) { perror("Error de open."); }
	
	/* Make a non-blocking request to place a write lock on bytes 0 to EOF of testfile */
	fl.l_type = F_WRLCK; 
	fl.l_whence = SEEK_SET; 
	fl.l_start = 0;
	fl.l_len = 0;
	fl.l_pid = getpid(); 


	if(fcntl(fd, F_GETLK, &fl) < 0)
	{ perror("Error fcntl.\n"); }


	
	if(fl.l_type == F_RDLCK) printf("READ LOCK.\n");	
	else if(fl.l_type == F_WRLCK) printf("WRITE LOCK.\n");
	else{	
		/*fijar cerrojo de escritura*/
		fl.l_type = F_WRLCK;
		if(fcntl(fd, F_SETLK, &fl) < 0) { 
			perror("Error al poner cerrojo de escritura."); 			
		}
		time_t t = time(NULL);
		printf("Hora actual: %s", ctime(&t));
		sleep(30);
		/*Liberamos el cerrojo*/
		fl.l_type = F_UNLCK;
		if(fcntl(fd, F_SETLK, &fl) < 0) { 
			perror("Error al liberar el cerrojo."); 
			
		}
		printf("Libero el cerrojo\n");
	}
	close(fd);
	return 0;
}
