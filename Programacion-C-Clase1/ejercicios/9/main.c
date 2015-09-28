#include <stdio.h>
#include <assert.h>
#include "utils.h"

static int test1(void)
{
	char cadena[15] = "Holaaa\n";
	char letra = 'a';
	int resultado11 = -1;
	int resultado12 = -1;
	int resultado13 = -1;

	resultado11 = cuenta_numero_letras(cadena, letra);
	assert(resultado11 == 5);

	resultado12 = compara_cadena(cadena, cadena);
	assert(resultado12 = 1);

	resultado13 = tipo_de_letra(letra);
	assert(resultado13 = 1);

	return 0;
}

static int test2(void)
{
	char cadena[30] = "Mi moto buena\n";
	char letra = 'a';
	int resultado14 = -1;
	int resultado16 = -1;

	resultado14 = primera_letra(cadena, letra);
	assert(resultado14 == 9);

	resultado16 = suma_numeros(1, 2);
	assert(resultado16 == 3);

	return 0;
}

static int test3(void)
{
	int n = 3;
	int m = 2;
	int resultado17 = -1;
	int resultado18 = -1;

	resultado17 = resta_numeros(n, m);
	assert(resultado17 == 1);

	resultado18 = multiplica_numeros(n, m);
	assert(resultado18 == 6);

	return 0;
}

void main(void)
{
	char cadena[15] = "Holaaaaa\n";
	char letraoriginal = 'a';
	char letracambiar = 'e';
	int resultado15 = -1;

	test1;
	test2;
	test3;

	resultado15 = cambia_letras(cadena, letraoriginal, letracambiar);
	assert(resultado15 == 0);
}
