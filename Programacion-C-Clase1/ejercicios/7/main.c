/* En el programa main, dado un numero 'n' con el valor que nosotros queramos
 * definir. Comprobar si ese numero es par usando la funcion definida en
 * utils.
 * Si ese numero es par, debemos multiplicarlo por otro numero secreto que
 * nosotros definimos. Para ello debemos realizar una función de tipo estática
 * llamada multiplicar_numero_secreto donde el argumento de entrada sea el
 * numero 'n'.
 *
 * Devolver la multiplicacion del numero 'n' con el numero secreto en caso de
 * que 'n' sea par. Si no devolver -1.
 */

/* Zona para incluir las bibliotecas o las cabeceras */
#include <stdio.h>
#include "utils.h"

/* Zona para declarar la funcion estática */
static int multiplicar_numero_secreto(int n)
{
	int numero_secreto = 2;

	return n * numero_secreto;
}

/* Programa Principal */
int main (void)
{
	int n = 1972;

	if (es_par(n))
		return multiplicar_numero_secreto(n);

	return -1;
}
