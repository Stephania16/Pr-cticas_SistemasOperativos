#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>


int main(){

	int fd;
	fd = open("./ejercicio3", O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IXOTH);
	if(fd == -1){ perror("Error open");}

	return 0;
}
