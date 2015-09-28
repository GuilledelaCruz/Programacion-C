#include "gestor_tarea.h"
#include <string.h>

struct gestor_tarea {
        struct tarea      *tareas[10];
	uint16_t	numero_tareas;

        uint32_t	flags;
};

struct gestor_tarea *gestor_tarea_alloc(void)
{
        return (struct gestor_tarea *)malloc(sizeof(struct gestor_tarea));
}

void gestor_tarea_free(struct gestor_tarea *g)
{
	int i;

	for(i = 0; i < g->numero_tareas; i++)
		xfree(g->tareas[i]);

	xfree(g);
}

void removeNulls(struct gestor_tarea *g, uint16_t n)
{
	int i;

	for (i = n; i < g->numero_tareas; i++) {
		g->tareas[i] = g->tareas[i + 1];
	}
}

void gestor_tarea_attr_unset_tarea(struct gestor_tarea *g, uint16_t pos)
{
        if (pos < 0 || pos >= g->numero_tareas)
                return;

        g->numero_tareas--;
	tarea_free(g->tareas[pos]);

	removeNulls(g, pos);

	if (g->numero_tareas == 0) {
        	g->flags &= ~(1 << GESTOR_TAREA_ATTR_TAREAS);
		g->flags &= ~(1 << GESTOR_TAREA_ATTR_NUMERO_TAREAS);
	}
}

void sortFunction(struct gestor_tarea *g, uint16_t n)
{
        int i,j;
        struct tarea *temp;

        for (i = 1; i < n; i++) {
                for (j = 0; j < n - i; j++) {
                        if (tarea_attr_get_u16(g->tareas[j],
                                TAREA_ATTR_PRIORIDAD) >
                                tarea_attr_get_u16(g->tareas[j + 1],
                                TAREA_ATTR_PRIORIDAD))
                        {
                                temp = g->tareas[j];
                                g->tareas[j] = g->tareas[j + 1];
                                g->tareas[j + 1] = temp;
                        }
                }
        }
}

void gestor_tarea_attr_set_tarea(struct gestor_tarea *g, uint16_t attr,
				 struct tarea *data)
{
	if (attr != GESTOR_TAREA_ATTR_TAREAS)
                return;

	if (g->numero_tareas > 9) {
               printf("El gestor está al máximo de tareas\n");
               return;
        }

        g->tareas[g->numero_tareas] = (struct tarea *)data;
        g->numero_tareas++;

	sortFunction(g, g->numero_tareas);

        g->flags |= (1 << GESTOR_TAREA_ATTR_TAREAS);
	g->flags |= (1 << GESTOR_TAREA_ATTR_NUMERO_TAREAS);
}

const void *gestor_tarea_attr_get_data(struct gestor_tarea *g, uint16_t attr,
					uint16_t pos)
{
	if (!(g->flags & (1 << attr)))
                return NULL;

        switch(attr) {
        case GESTOR_TAREA_ATTR_TAREAS:
                return g->tareas[pos];
        case GESTOR_TAREA_ATTR_NUMERO_TAREAS:
                return &g->numero_tareas;
        }
        return NULL;
}

const struct tarea *gestor_tarea_attr_get_tarea(struct gestor_tarea *g,
						uint16_t attr, uint16_t pos)
{
        return (struct tarea *)gestor_tarea_attr_get_data(g, attr, pos);
}

const uint16_t gestor_tarea_attr_get_u16(struct gestor_tarea *g,
					uint16_t attr)
{
	const void * ret = gestor_tarea_attr_get_data(g, attr, 0);
	return ret == NULL ? 0 : *((uint16_t *)ret);
}

int gestor_tarea_snprintf(char *buf, size_t size, struct gestor_tarea *g)
{
	int i,ret = 0;

	for (i = 0; i < g->numero_tareas; i++) {
		ret += tarea_snprintf(buf + ret, size - ret, g->tareas[i]);
                ret += snprintf(buf + ret, size - ret, "\n");
	}

	return ret;
}
