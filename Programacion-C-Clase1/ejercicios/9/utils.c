#include <string.h>
#include "utils.h"

int cuenta_numero_letras(char cadena[], char letra)
{
	int i;
	int contador = 0;

	for(i = 0; i <= strlen(cadena); i++){
		if (cadena[i] == letra)
			contador++;
	}

	return contador;
}

int compara_cadena(char * cadena1, char * cadena2)
{
	int resultado = 1;
	int i;

	if(strlen(cadena1) == strlen(cadena2))
		resultado = strcmp(cadena1, cadena2);
	else
		resultado = 0;

	return resultado;
}

int tipo_de_letra(char letra)
{
	int tipo;

	switch(letra){
	case 'a':
		tipo = 1;
		break;
	case 'c':
		tipo = 2;
		break;
	case 'e':
		tipo = 3;
		break;
	default:
		tipo = 0;
		break;
	}

	return tipo;
}

int primera_letra(char cadena[], char letra)
{
	int posicion = -1;
	int i;

	for(i = 0; i <= strlen(cadena); i++){
		if (cadena[i] == letra){
			posicion = 0;
			break;
		}else{
			continue;
		}
	}

	return posicion;
}

int cambia_letras(char cadena[], char letraoriginal, char letracambiar)
{
	int i;

	for(i = 0; i <= strlen(cadena); i++){
		if (cadena[i] == letraoriginal)
			cadena[i] = letracambiar;
	}

	return 0;
}

int suma_numeros(int n, int m)
{
	return (n + m);
}

int resta_numeros(int n, int m)
{
	return (n - m);
}

int multiplica_numeros(int n, int m)
{
	return (n * m);
}
