#include "../include/concesionario.h"
#include <string.h>

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


struct concesionario {
	struct list_head	garaje;
	const char		*dueno;
	uint32_t		num_coches;

	uint32_t		flags;
};

struct concesionario *curso_concesionario_alloc(void)
{
	struct concesionario *con;
	char *d = "Ningun dueño asignador";
	con = (struct concesionario *)malloc(sizeof(struct concesionario));
	if (con ==  NULL)
		return NULL;

	INIT_LIST_HEAD(&con->garaje);
	con->num_coches = 0;
	con->dueno = strdup(d);
	con->flags = 0;

	con->flags |= 1 << CURSO_CONCESIONARIO_ATTR_NUM_COCHES;

	return con;
}

void curso_concesionario_free(struct concesionario *con)
{
	struct coche *c, *tmp;

	if (con->flags & (1 << CURSO_CONCESIONARIO_ATTR_DUENO))
		xfree(con->dueno);

	list_for_each_entry_safe(c, tmp, &con->garaje, head) {
		list_del(&c->head);
		curso_coche_free(c);
	}

	xfree(con);
}

void curso_concesionario_attr_unset_coche(struct concesionario *con,
					  uint32_t pos)
{
	int i = 0;
	struct coche *c, *tmp;

	if (pos < 0 || pos > con->num_coches)
		return;

	list_for_each_entry_safe(c, tmp, &con->garaje, head) {
		if (i == pos) {
			list_del(&c->head);
			curso_coche_free(c);
			break;
		}

		i++;
	}

	con->num_coches--;
}

static void curso_concesionario_set_data(struct concesionario *con,
					 uint16_t attr, const void *data)
{
	struct coche *c;

	if (attr > CURSO_CONCESIONARIO_ATTR_MAX)
		return;

	switch (attr) {
	case CURSO_CONCESIONARIO_ATTR_DUENO:
		if (con->dueno)
			xfree(con->dueno);

		con->dueno = strdup(data);
		break;
	case CURSO_CONCESIONARIO_ATTR_COCHE:
		if (con->num_coches > 50) {
			printf("El garaje esta lleno\n");
			break;
		}
		c = (struct coche *)data;
		list_add_tail(&c->head, &con->garaje);
		con->num_coches++;
		break;
	}

	con->flags |= (1 << attr);
}

void curso_concesionario_attr_set_str(struct concesionario *con,
					uint16_t attr, const char *data)
{
	curso_concesionario_set_data(con, attr, data);
}

void curso_concesionario_attr_set_coche(struct concesionario *con,
					  uint16_t attr, struct coche *data)
{
	curso_concesionario_set_data(con, attr, data);
}

const void *curso_concesionario_attr_get_data(struct concesionario *con,
					      uint16_t attr, uint32_t pos)
{
	int i = 0;
	struct coche *c;

	if (!(con->flags & (1 << attr)))
		return NULL;

	switch(attr) {
	case CURSO_CONCESIONARIO_ATTR_DUENO:
		return con->dueno;
	case CURSO_CONCESIONARIO_ATTR_NUM_COCHES:
		return &con->num_coches;
	case CURSO_CONCESIONARIO_ATTR_COCHE:
		list_for_each_entry(c, &con->garaje, head) {
			if (i == pos)
				break;

			i++;
		}

		return c;
	}
	return NULL;
}

uint32_t curso_concesionario_attr_get_u32(struct concesionario *con,
					  uint16_t attr)
{
	const void *ret = curso_concesionario_attr_get_data(con, attr, 0);
	return ret == NULL ? 0 : *((uint32_t *)ret);
}

const char *curso_concesionario_attr_get_str(struct concesionario *con,
					     uint16_t attr)
{
	return curso_concesionario_attr_get_data(con, attr, 0);
}

struct coche *curso_concesionario_attr_get_coche(struct concesionario *con,
					         uint16_t attr, uint32_t pos)
{
	return (struct coche *)curso_concesionario_attr_get_data(con, attr,
								 pos);
}

int curso_concesionario_snprintf(char *buf, size_t size,
				 struct concesionario *con)
{
	int ret = 0;
	struct coche *c;

	ret += snprintf(buf, size,
			"el concesionario propiedad de %s, tiene %d y son:\n",
			con->dueno, con->num_coches);

	list_for_each_entry(c, &con->garaje, head) {
		ret += curso_coche_snprintf(buf + ret, size - ret, c);
		ret += snprintf(buf + ret, size - ret, "\n");
	}

	return ret;
}

const void *curso_concesionario_write_coches(struct concesionario *con,
						FILE *file)
{
	struct coche *c;
	char *matricula, *marca;
	char *id;
	char *buffer;

	id = (char *)calloc(10,  sizeof(char *));
	matricula = (char *)calloc(50, sizeof(char *));
	marca = (char *)calloc(50, sizeof(char *));
	buffer = (char *)calloc(120, sizeof(char *));

	list_for_each_entry(c, &con->garaje, head) {
		buffer = strcpy(buffer, "");
		sprintf(id, "%d", c->id);
		buffer = strcat(buffer, id);
		buffer = strcat(buffer, ",");
		matricula = strcpy(matricula, c->matricula);
		buffer = strcat(buffer, matricula);
                buffer = strcat(buffer, ",");
		marca = strcpy(marca, c->marca);
		buffer = strcat(buffer, marca);
                buffer = strcat(buffer, "\n");
		fwrite(buffer, strlen(buffer), 1, file);
	}

	xfree(id);
	xfree(matricula);
	xfree(marca);
	xfree(buffer);
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
