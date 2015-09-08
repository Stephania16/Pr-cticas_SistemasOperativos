/*fork*/
#include <unistd.h>
/*printf*/
#include <stdio.h>
/*getsid*/
#include <sys/time.h>
/*getrlimit*/
#include <sys/types.h>
#include <sys/resource.h>
/*perror*/
#include <errno.h>


int main(){

	pid_t f;
	pid_t pid, ppid, pgid, sid;
	struct rlimit rl;
	char buf[100];	
	
	f = fork();
	if(f == -1) /*Error*/
		perror("ERROR FORK.\n");
	else if(f == 0) /*Hijo*/
	{
		printf("Nueva sesión: %d\n", setsid());
		chdir("/tmp");
		pid = getpid();
		ppid = getppid();
		pgid = getpgid(pid);
		sid = getsid(pid);
		printf("Pid del proceso: %d\n", pid);
		printf("Pid del padre del proceso: %d\n", ppid);
		printf("Pid del grupo: %d\n", pgid);
		printf("Pid de sesión: %d\n", sid);
		printf("Máximo tamaño de archivo (bytes): %d\n", getrlimit(RLIMIT_FSIZE, &rl));
		printf("Directorio de tamaño actual: %s\n", getcwd(buf, 100));
		
	}

	else {/*Padre*/}

	return 0;
}
