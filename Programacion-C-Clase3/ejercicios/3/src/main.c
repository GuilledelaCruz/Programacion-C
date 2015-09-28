#include "../include/concesionario.h"
#include <getopt.h>
#include <string.h>

void read_file_to_concesionario(const char *file, struct concesionario *con);
void write_concesionario_to_file(const char *file,
				 struct concesionario *con);

int nextAttr(FILE *f, char *cadena)
{
	int pos;
	char ch = fgetc(f);

	for (pos = 0; ch != '\n' && ch > 0 && pos < 50 && ch != ','; pos++){
                cadena[pos] = ch;
		ch = fgetc(f);
        }
        cadena[pos] = '\0';

	return pos;
}

int main(int argc, char *argv[])
{
	int valor, option_index = 0;
	struct coche *c1;
	struct concesionario *con;
	char path[250];
	char *salida = "../files/salida.txt";

	uint8_t flag = 0;

	static struct option long_options[] = {
		{ "file", required_argument, 0, 'f' },
		{ 0 }
	};

	while((valor = getopt_long(argc, argv, "f:", long_options,
				&option_index)) != -1) {

		switch(valor) {
		case 'f':
			strncpy(path, optarg, 249);
			flag |= 1 << 0;
			break;
		default:
			printf("El parametro de entrada es -f/-file "
				"{archivo}\n");
			break;
		}
	}

	if (flag != 1) {
		printf("Los parametros de entrada no son correctos\n"
			"Prueba con -f/-file {nombrearchivo}\n");
		return 0;
	}

	con = curso_concesionario_alloc();

	read_file_to_concesionario(path, con);

	write_concesionario_to_file(salida, con);

	curso_concesionario_free(con);
	return 0;
}

void read_file_to_concesionario(const char *file, struct concesionario *con)
{
	FILE *f;
	struct coche *c1;
	char dueno[50], matricula[50], gama[50];
	char buffer[100];
	char id[5];
	int br = 0;
	int max_coches = 0;

	f = fopen(file, "r");
	if (f == NULL) {
		printf("No se pudo leer el archivo %s\n", file);
		return;
	}

	br = nextAttr(f, dueno);

	curso_concesionario_attr_set_str(con, CURSO_CONCESIONARIO_ATTR_DUENO,
                                         dueno);

	while (max_coches < 10) {
		c1 = curso_coche_alloc();

		br = nextAttr(f, id);
                curso_coche_attr_set_u32(c1, CURSO_COCHE_ATTR_ID, atoi(id));

		br = nextAttr(f, matricula);
        	curso_coche_attr_set_str(c1, CURSO_COCHE_ATTR_MATRICULA,
					matricula);

		br = nextAttr(f, gama);
        	curso_coche_attr_set_str(c1, CURSO_COCHE_ATTR_MARCA, 
					gama);

		if (br < 1) {
			curso_coche_free(c1);
			break;
		}

		curso_concesionario_attr_set_coche(con,
					CURSO_CONCESIONARIO_ATTR_COCHE, c1);	

		max_coches++;

	}

	rewind(f);

	if (fclose(f) == EOF){
		printf("No se pudo cerrar el archivo %s\n", file);
		return;
	}
}

void write_concesionario_to_file(const char *file, 
				 struct concesionario *con)
{
	FILE *f;
	struct coche *c;
	uint16_t pos;
	char *dueno = (char *)calloc(50, sizeof(char *));

	f = fopen(file, "w+");
	if (f == NULL) {
		printf("No se pudo guardar el archivo %s\n", file);
		xfree(dueno);
		return;
	}

	dueno = strcpy(dueno, curso_concesionario_attr_get_str(con,
                        CURSO_CONCESIONARIO_ATTR_DUENO));

        strcat(dueno, "\n");

	fwrite(dueno, strlen(dueno), 1, f);
	curso_concesionario_write_coches(con, f);

	if (fclose(f) == EOF){
                printf("No se pudo cerrar el archivo %s\n", file);
		xfree(dueno);
                return;
        }
	xfree(dueno);
}
