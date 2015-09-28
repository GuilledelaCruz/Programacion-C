/* Creando una variable local de nombre "valor". Comprobar si dicha variable
 * es 0, si es así imprimir un mensaje diciendo que se ha recibido un 0 o un 1.
 * En los demas casos imprimir un mensaje diciendo que no es un valor esperado.
 */

/* Bibliotecas a incluir */
#include <stdio.h>

void main(void){
	/* Declaración de variables */
	int valor = 0;

	/* Código usando condicional switch */
	switch (valor) {
	case 0:
	case 1:
		printf("Valor es 0 ó 1.\n");
		break;
	default:
		printf("Valor inesperado.\n");
		break;
	}
}
