#include <stdio.h>
#include <stdlib.h>

void genera_mensaje(FILE *file)
{
	if (fputs("El mensaje a escribir es Alvaro", file) == EOF)
		return;
	rewind(file);
}

int main (void)
{
	FILE *file = fopen("datos.dat", "a+");
	char *code;
	size_t n = 0;
	int c;

	if (file == NULL)
		return -1;

	genera_mensaje(file);

	code = malloc(100);
	if (code == NULL)
		return -1;

	while(fgets(code, 100, file)) {
		printf("%s\n", code);
	}

	free(code);
	if (!fclose(file))
		return -1;

	return 0;
}
