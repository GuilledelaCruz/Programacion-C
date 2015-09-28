/* Recorrer una cadena de caracteres de nombre "cadena". Comprobar si la
 * posicion i de la cadena es un igual a una letra 'a'. Si no es así continuamos
 * buscando. Si es así, debemos comprobar si i+1 es una letra 's'. Si es una
 * 's' iremos a una etiqueta de nombre "es_s" la cual imprimimos que tenemos
 * una 'a' seguida de una 's' y devolvemos un 1. Si no es una letra 's'.
 * Finalizamos la ejecución del for, imprimimos un mensaje del tipo no tenemos
 * una 'a' seguida de una 's' y devolvemos 0.

/* Bibliotecas a incluir */
#include <stdio.h>
#include <string.h>

int main(void)
{
	/* Declaración de variables */
	char cadena[10] = "cadena\n";
	int i;

	/* Código usando condicional for */
	for (i = 0; cadena[i] != '\0' && i < strlen(cadena); i++) {
		if (cadena[i] == 'a') {
			if (cadena[i + 1] == 's')
				goto es_s;
			break;
		}
	}

	printf("No tenemos una 'a' seguida de una 's'\n");
	return 0;

	/* Declaracion de etiquetas */
	es_s:
		printf("Tenemos una 'a' seguida de una 's'\n");
		return 1;
}
