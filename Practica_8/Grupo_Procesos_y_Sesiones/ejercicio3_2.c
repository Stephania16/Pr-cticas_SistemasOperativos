/*fork*/
#include <unistd.h>
/*printf*/
#include <stdio.h>
/*perror*/
#include <errno.h>


int main(){

	pid_t f;
	pid_t pid, ppid, pgid, sid;	
	
	f = fork();
	if(f == -1) /*Error*/
		perror("ERROR FORK.\n");
	else if(f == 0) /*Hijo*/
	{
		sleep(1);
		pid = getpid();
		printf("Pid del hijo: %d\n", pid);	
	}

	else {/*Padre*/
		pid = getpid();
		printf("Pid del padre: %d\n", pid);
	}

	return 0;
}
