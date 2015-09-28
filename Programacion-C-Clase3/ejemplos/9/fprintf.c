#include <stdio.h>
#include <stdlib.h>

void genera_mensaje(FILE *file)
{
	int linea;

	for (linea = 0; linea < 6; linea++) {
		fprintf(file, "Linea %d: Mensaje de prueba\n", linea);
	}
	rewind(file);
}

int main (void)
{
	FILE *file = fopen("datos.dat", "a+");
	char *code;
	size_t n = 0;

	if (file == NULL)
		return -1;

	genera_mensaje(file);

	code = malloc(100);
	if (code == NULL)
		return -1;

	while(fgets(code, 100, file)) {
		printf("%s", code);
	}

	free(code);
	if (!fclose(file))
		return -1;
	return 0;
}
