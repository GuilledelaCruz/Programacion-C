#include <stdio.h>
#include <string.h>

int main(void)
{
	FILE *fp;
	char c[] = "ejemplo fwrite/fread";
	char buffer[20];
	int escritos;

	fp = fopen("datos.dat", "w+");
	if (fp == NULL)
		return -1;

	escritos = fwrite(c, strlen(c) + 1, 1, fp);

	rewind(fp);

	if (fread(buffer, strlen(c) + 1, 1, fp) != escritos)
		return -1;

	printf("%s\n", buffer);

	if (!fclose(fp))
		return -1;
	return 0;
}
