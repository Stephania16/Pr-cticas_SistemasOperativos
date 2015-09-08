#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv){

	int fd;
	fd = open("./tub", O_WRONLY);
	
	dup2(fd,0);
	printf("Hola\n");

	//scanf("%s",argv[1]);
	close(fd);
	return 0;
}
