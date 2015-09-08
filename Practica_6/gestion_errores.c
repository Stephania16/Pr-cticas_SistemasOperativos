/*setuid*/
#include <sys/types.h>
#include <unistd.h>
/*perror, printf*/
#include <stdio.h>
#include <errno.h>
/*strcmp, strerror*/
#include <string.h>

int main(){
  int valor;
  valor = setuid(0);
/*Si steuid da error mostramos el código de error generado, en su    versión numérica y la cadena asociada*/
  if(valor == -1)
  {	perror("ERROR SETUID");
	printf("Número del error es: %i\n", errno);
  }

  printf("***Imprimiendo mensajes de error disponibles en el sistema ***\n");
/*Recorremos un bucle que imprima todos los mensajes de error disponibles en el sistema*/	
  int i = 1;
  while(strcmp(strerror(i), strerror(i+1)) != 0)
  {
	printf("ERROR %d: %s\n", i, strerror(i));
	i++;
  }
  return 0;
}
