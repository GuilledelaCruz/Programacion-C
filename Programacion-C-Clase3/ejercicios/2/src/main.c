#include "../include/concesionario.h"
#include <getopt.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int valor, option_index = 0;
	struct coche *c1, *c2;
	struct concesionario *con;
	char info[4000];
	char matricula[15];
	char marca[50];
	uint32_t id = 0;
	char dueno[50];

	uint8_t campos = 0;

	static struct option long_options[] = {
                { "matricula", required_argument, 0, 'm' },
                { "id", required_argument, 0, 'i' },
                { "gama", required_argument, 0, 'g' },
		{ "dueno", required_argument, 0, 'd'},
		{ "help", 0, 0, 'h'},
                { 0 }
        };

	while ((valor = getopt_long(argc, argv, "m:i:g:d:h", long_options, 
                        &option_index)) != -1) {

        	switch(valor) {
        	case 'm':
			strncpy(matricula, optarg, 14);
			campos |= 1 << 0;
                	break;
        	case 'i':
			id = atoi(optarg);
			campos |= 1 << 1;
                	break;
        	case 'g':
			strncpy(marca, optarg, 49);
			campos |= 1 << 2;
                	break;
		case 'd':
			strncpy(dueno, optarg, 49);
			campos |= 1 << 3;
			break;
		case 'h':
			printf("Los parametros a usar son los siguientes:\n"
				"-m/matricula <matricula coche>\n"
				"-i/id <id coche>\n"
				"-g/-gama <modelo coche>\n"
				"-d/-dueno <dueño concesionario>\n");
			break;
        	default:
               		printf("Debe usar el parametro -m/-matricula, -i/-id,"
                       		" -g/-gama, -d/-dueño o -h/-help.\n");
                	break;
       		}
	}

	if (campos != 15) {
		printf("Los campos introducidos no son validos.\n");
		return 0;
	}

	c1 = curso_coche_alloc();

	curso_coche_attr_set_str(c1, CURSO_COCHE_ATTR_MATRICULA, matricula);
	curso_coche_attr_set_str(c1, CURSO_COCHE_ATTR_MARCA, marca);
	curso_coche_attr_set_u32(c1, CURSO_COCHE_ATTR_ID, id);

	con = curso_concesionario_alloc();

	curso_concesionario_attr_set_str(con, CURSO_CONCESIONARIO_ATTR_DUENO,
					 dueno);

	curso_concesionario_attr_set_coche(con, CURSO_CONCESIONARIO_ATTR_COCHE,
					   c1);

	curso_concesionario_snprintf(info, sizeof(info), con);
	printf("%s", info);

	curso_concesionario_free(con);
	return 0;
}
