#include "coche.h"

enum {
	CURSO_CONCESIONARIO_ATTR_COCHE,
	CURSO_CONCESIONARIO_ATTR_DUENO,
	CURSO_CONCESIONARIO_ATTR_NUM_COCHES,
	__CURSO_CONCESIONARIO_ATTR_MAX
};

#define CURSO_CONCESIONARIO_ATTR_MAX (__CURSO_CONCESIONARIO_ATTR_MAX - 1)

struct concesionario;

struct concesionario *curso_concesionario_alloc(void);
void curso_concesionario_free(struct concesionario *);

void curso_concesionario_attr_unset_coche(struct concesionario *con,
					  uint32_t pos);

void curso_concesionario_attr_set_str(struct concesionario *con, uint16_t attr,
				      const char *data);
void curso_concesionario_attr_set_coche(struct concesionario *con,
					uint16_t attr, struct coche *c);

uint32_t curso_concesionario_attr_get_u32(struct concesionario *con,
					  uint16_t attr);
const char *curso_concesionario_attr_get_str(struct concesionario *con,
					     uint16_t attr);
struct coche *curso_concesionario_attr_get_coche(struct concesionario *con,
						 uint16_t attr, uint32_t pos);

int curso_concesionario_snprintf(char *buf, size_t size,
				struct concesionario *c);

const void *curso_concesionario_write_coches(struct concesionario *con,
                                        FILE *file);

void read_file_to_concesionario(const char *file, struct concesionario *con);
void write_concesionario_to_file(const char *file,
                                 struct concesionario *con);
