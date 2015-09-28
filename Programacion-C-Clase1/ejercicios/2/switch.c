/* Creando una variable local de nombre "valor". Comprobar si dicha variable
 * es 0, si es así imprimir un mensaje diciendo que se ha recibido un 0.
 * Si es un uno, mensaje confirmando que se ha recibido un uno. En los demas
 * casos imprimir un mensaje diciendo que no es un valor esperado.
 */

/* Bibliotecas a incluir */
#include <stdio.h>

void main(void){
	/* Declaración de variables */
	int valor = 2;

	/* Código usando condicional switch */
	switch (valor) {
	case 0:
		printf("Valor es igual a 0.\n");
		break;
	case 1:
		printf("Valor es igual a 1.\n");
		break;
	default:
		printf("Valor es distinto de 0 y 1.\n");
		break;
	}
}
