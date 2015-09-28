#include <stdio.h>
#include <stdlib.h>

void genera_mensaje(FILE *file)
{
	if (fputc('A', file) == EOF)
		return;

	if (fputc('l', file) == EOF)
                return;

	if (fputc('v', file) == EOF)
                return;

	if (fputc('a', file) == EOF)
                return;

	if (fputc('r', file) == EOF)
                return;

	if (fputc('o', file) == EOF)
                return;

	rewind(file);
}

int main(void)
{
	FILE *file = fopen("datos.dat", "a+");
	char *code;
	size_t n = 0;
	int c;

	if (file == NULL)
		return -1;

	genera_mensaje(file);

	code = malloc(1000);
	while ((c = fgetc(file)) != EOF)
	{
		code[n++] = (char) c;
	}

	code[n] = '\0';
	printf("El texto leido es %s\n", code);

	free(code);
	if (!fclose(file))
		return -1;

	return 0;
}
