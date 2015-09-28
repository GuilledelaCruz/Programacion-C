#include "tarea.h"

enum {
	GESTOR_TAREA_ATTR_TAREAS = 0,
	GESTOR_TAREA_ATTR_NUMERO_TAREAS,
	__GESTOR_TAREA_ATTR_MAX
};

#define GESTOR_TAREA_ATTR_MAX  (__GESTOR_TAREA_ATTR_MAX - 1)

struct gestor_tarea;

struct gestor_tarea *gestor_tarea_alloc(void);
void gestor_tarea_free(struct gestor_tarea *g);

void gestor_tarea_attr_unset_tarea(struct gestor_tarea *g, uint16_t pos);

void gestor_tarea_attr_set_tarea(struct gestor_tarea *g, uint16_t attr,
				 struct tarea *data);

const void *gestor_tarea_attr_get_data(struct gestor_tarea *g, uint16_t attr,
                                        uint16_t pos);
const struct tarea *gestor_tarea_attr_get_tarea(struct gestor_tarea *g,
					 uint16_t attr, uint16_t pos);
const uint16_t gestor_tarea_attr_get_u16(struct gestor_tarea *g,
					  uint16_t attr);

int gestor_tarea_snprintf(char *buf, size_t size, struct gestor_tarea *g);
