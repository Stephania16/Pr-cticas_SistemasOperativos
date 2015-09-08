/*time*/
#include <time.h>
/*gettimeofday*/
#include <sys/time.h>
/*printf*/
#include <stdio.h>

int main(){
	/*Ejercicio 2 - Mostrar la hora usando time()*/
	time_t hora = time(NULL);
	printf("Hora formato time: %ld\n", hora);
	
	/*Ejercicio 3 - Formato legible con ctime()*/
	printf("Hora formato legible: %s\n", ctime(&hora));
	
	/*Ejercicio 4 - Usando gettimeofday()*/
	int i = 0;
	int var = 1;
	struct timeval tim;
	gettimeofday(&tim, NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	for(i; i < 10000; i++)
	{var++;}
	gettimeofday(&tim, NULL);
	double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("%.6lf seconds elapsed\n", t2-t1);

	/*Ejercicio 5 - Usando localtime()*/
	struct tm *local;
	local = localtime(&hora);
        printf("Estamos en el año %d\n", local->tm_year+1900);

	/*Ejercicio 6 - Usando strftime()*/
	char buf[25];
	strftime(buf, sizeof(buf), "Hoy es %A, %H:%M", local);
        printf("%s\n", buf);
	
	return 0;
}
