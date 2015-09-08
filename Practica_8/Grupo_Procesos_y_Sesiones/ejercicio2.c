/*pid, getcwd*/
#include <unistd.h>
/*printf*/
#include <stdio.h>
/*getsid*/
#include <sys/time.h>
/*getrlimit*/
#include <sys/types.h>
#include <sys/resource.h>


int main(){

	pid_t pid, ppid, pgid, sid;
	struct rlimit rl;
	char buf[100];	

	pid = getpid();
	ppid = getppid();
	pgid = getpgid(pid);
	sid = getsid(pid);

	printf("Pid del proceso: %d\n", pid);
	printf("Pid del padre del proceso: %d\n", ppid);
	printf("Pid del grupo: %d\n", pgid);
	printf("Pid de sesión: %d\n", sid);
	printf("Máximo tamaño de archivo (bytes): %d\n", getrlimit(RLIMIT_FSIZE, &rl));
	printf("Directorio de trabajo actual: %s\n", getcwd(buf, 100));
	return 0;
}

