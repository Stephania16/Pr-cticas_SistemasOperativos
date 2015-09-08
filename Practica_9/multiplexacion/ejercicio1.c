#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/*Para probar lo que se ha leido de la tub1 y tub2, abrir otra terminal y enviar: 
	echo hola > tub1
	echo hola > tub2
 Se mostrará lo que se ha leído
*/

#define max(a,b) (((a) > (b)) ? (a) : (b))

int main(int argc, char **argv){
	char buffer[256];
	int fd1, fd2, read1, read2;
	fd_set set;
	struct timeval t;
	t.tv_sec = 2;
	t.tv_usec = 0;

	mkfifo("tub1", 0644);
	mkfifo("tub2", 0644);

	printf("Abrimos la primera tubería.\n");
	fd1 = open("tub1", O_RDONLY | O_NONBLOCK);

	printf("Abrimos la segunda tubería.\n");
	fd2 = open("tub2", O_RDONLY | O_NONBLOCK);

	if(fd1 < 0 || fd2 < 0) perror("Error al abrir las tuberías.\n");
	printf("Valor fd1: %d\n", fd1);
	printf("Valor fd2: %d\n", fd2);
	
	while(1){
		sleep(3);
		FD_ZERO(&set);
		FD_SET(fd1, &set);	
		FD_SET(fd2, &set);

		select(max(fd1,fd2) + 1, &set, NULL, NULL, &t);

		if(FD_ISSET(fd1, &set))
		{
			read1 = read(fd1, buffer, 256);
			if(read1 < 0) perror("Error de read1.\n");
			printf("tub1:");
			buffer[read1] = '\0';
			printf("%s", buffer);
			close(fd1);
			fd1 = open("./tub1", O_RDONLY | O_NONBLOCK);
		}

		if(FD_ISSET(fd2, &set))
		{
			read2 = read(fd2, buffer, 256);
			if(read2 < 0) perror("Error de read2.\n");
			printf("tub2:");
			buffer[read2] = '\0';
			printf("%s", buffer);
			close(fd2);
			fd2= open("./tub2", O_RDONLY | O_NONBLOCK);
		}

	}
	return 0;
}
