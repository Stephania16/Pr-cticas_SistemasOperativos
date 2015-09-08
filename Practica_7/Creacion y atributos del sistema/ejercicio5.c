#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>


int main(){

	int fd;
	umask(S_IWGRP | S_IROTH | S_IWOTH | S_IXOTH);
	fd = open("./ejercicio5_prueba", O_CREAT);
	if(fd == -1){ perror("Error open");}

	return 0;
}
