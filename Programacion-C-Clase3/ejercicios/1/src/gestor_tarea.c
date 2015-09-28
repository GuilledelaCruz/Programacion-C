#include "../include/gestor_tarea.h"
#include <string.h>

struct gestor_tarea {
        struct list_head      tareas;
	uint16_t	numero_tareas;

        uint32_t	flags;
};

struct gestor_tarea *gestor_tarea_alloc(void)
{
	struct gestor_tarea *g;
	g = (struct gestor_tarea *)malloc(sizeof(struct gestor_tarea));

	if (g == NULL)
		return NULL;

	INIT_LIST_HEAD(&g->tareas);
	g->numero_tareas = 0;
	g->flags = 0;

        return g;
}

void gestor_tarea_free(struct gestor_tarea *g)
{
	struct tarea *t, *tmp;

	list_for_each_entry_safe(t, tmp, &g->tareas, head) {
                list_del(&t->head);
                tarea_free(t);
        }

	xfree(g);
}

void gestor_tarea_attr_unset_tarea(struct gestor_tarea *g, uint16_t pos)
{
	int i = 0;
	struct tarea *t, *tmp;

        if (pos < 0 || pos >= g->numero_tareas)
                return;

	list_for_each_entry_safe(t, tmp, &g->tareas, head) {
		if (i == pos) {
			list_del(&t->head);
			tarea_free(t);
			break;
		}

		i++;
	}

        g->numero_tareas--;

	if (g->numero_tareas == 0) {
        	g->flags &= ~(1 << GESTOR_TAREA_ATTR_TAREAS);
		g->flags &= ~(1 << GESTOR_TAREA_ATTR_NUMERO_TAREAS);
	}
}

void gestor_tarea_attr_set_tarea(struct gestor_tarea *g, uint16_t attr,
				 struct tarea *data)
{
	struct tarea *t;

	if (attr != GESTOR_TAREA_ATTR_TAREAS)
                return;

	if (g->numero_tareas > 9) {
               printf("El gestor está al máximo de tareas\n");
               return;
        }

	t = (struct tarea *)data;
        list_add(&t->head, &g->tareas);
        g->numero_tareas++;

        g->flags |= (1 << GESTOR_TAREA_ATTR_TAREAS);
	g->flags |= (1 << GESTOR_TAREA_ATTR_NUMERO_TAREAS);
}

const void *gestor_tarea_attr_get_data(struct gestor_tarea *g, uint16_t attr,
					uint16_t pos)
{
	int i = 0;
	struct tarea *t;

	if (!(g->flags & (1 << attr)))
                return NULL;

        switch(attr) {
        case GESTOR_TAREA_ATTR_TAREAS:
                list_for_each_entry(t, &g->tareas, head) {
                        if (i == pos)
                                break;

                        i++;
                }

                return t;

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
	int ret = 0;
	struct tarea *t;

	list_for_each_entry(t, &g->tareas, head) {
                ret += tarea_snprintf(buf + ret, size - ret, t);
                ret += snprintf(buf + ret, size - ret, "\n");
        }

	return ret;
}
