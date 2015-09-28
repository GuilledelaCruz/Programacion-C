/* Declarar una funcion que devuelva un entero. Dicha funcion se llamara
 * "es_par". Dicha funcion le entra un argumento de tipo entero que se llama
 * numero. La funcion devolvera 1 si el modulo del numero da 0. En caso
 * contrario 0.
 *
 * Dicha funcion sera exportada para utilizarla en otros ficheros.
 */

/* Zona para incluir las bibliotecas o cabeceras */
#include "utils.h"

/* Declaracion de la funcion */
int es_par(int numero)
{
	if (numero % 2 == 0)
		return 1;
	else
		return 0;	
}
