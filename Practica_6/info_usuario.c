/*getuid, getgid, geteuid, getegid, setuid*/
#include <unistd.h>
/*getpwuid*/
#include <sys/types.h>
#include <pwd.h>
/*perror*/
#include <stdio.h>

int main(){

	struct passwd *pw;

	/*Ejercicio 2*/
	printf("Id real del usuario: %d\n", getuid());
	printf("Id efectivo del usuario: %d\n", geteuid());


	/*Ejercicio 3*/
	pw = getpwuid(getuid());
	
	printf("Nombre de login: %s\n", pw->pw_name);
	printf("Nombre de directorio HOME: %s\n", pw->pw_dir);
	printf("Usuario real (UID): %d\n", pw->pw_uid);
	printf("SHELL: %s\n", pw->pw_shell);

	/*Activado el setuid*/
	
	if(setuid(2) == -1){
		perror("ERROR:");
		return -1;
	}

	printf("Id real: %d\n", getuid());
	printf("Id efectivo: %d\n", geteuid());

	return 0;
}
