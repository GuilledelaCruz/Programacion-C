/* Creando una variable local de nombre "valor". Comprobar si dicha variable
 * es mayor que 5, si es así imprimir un mensaje usando printf confirmando
 * que es mayor. En caso contrario, escribir un mensaje
 */

/* Bibliotecas a incluir */
#include <stdio.h>

void main(void) {
	/* Declaración de variables */
	int valor = 0;

	/* Código usando condicional if */
	if (valor > 5)
		printf("La variable 'valor' es mayor que 5\n");
	else
		printf("La variable 'valor' es menor o igual que 5\n");
}
