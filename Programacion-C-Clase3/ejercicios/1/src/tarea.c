#include "../include/tarea.h"
#include <string.h>

struct tarea *tarea_alloc(void)
{
	struct tarea *t = (struct tarea *)malloc(sizeof(struct tarea));

	if (t == NULL)
		return NULL;

	t->nombre = NULL;
	t->descripcion = NULL;
	t->id = 0;
	t->usuario = NULL;
	t->prioridad = 20;
	t->flags = 0;

	return t;
}

void tarea_free(struct tarea *t)
{
	if (t->flags & (1 << TAREA_ATTR_NOMBRE))
		xfree(t->nombre);

	if (t->flags & (1 << TAREA_ATTR_DESCRIPCION))
		xfree(t->descripcion);

	if (t->flags & (1 << TAREA_ATTR_USUARIO))
		xfree(t->usuario);

	xfree(t);
}

bool tarea_attr_is_set(const struct tarea *t, uint16_t attr)
{
	return t->flags & (1 << attr);
}

void tarea_attr_unset(struct tarea *t, uint16_t attr)
{
	if (!(t->flags & (1 << attr)))
		return;

	switch(attr) {
	case TAREA_ATTR_NOMBRE:
		if (t->nombre){
			xfree(t->nombre);
			t->nombre = NULL;
		}
		break;
	case TAREA_ATTR_DESCRIPCION:
		if (t->descripcion){
			xfree(t->descripcion);
			t->descripcion = NULL;
		}
		break;
	case TAREA_ATTR_ID:
		break;
	case TAREA_ATTR_USUARIO:
		if (t->usuario){
			xfree(t->usuario);
			t->usuario = NULL;
		}
		break;
	case TAREA_ATTR_PRIORIDAD:
		break;
	}

	t->flags &= ~(1 << attr);
}

void tarea_attr_set_data(struct tarea *t, uint16_t attr, const void *data,
                               uint32_t data_len)
{
	if (attr > TAREA_ATTR_MAX)
		return;

	switch(attr) {
	case TAREA_ATTR_NOMBRE:
		if (t->nombre)
			xfree(t->nombre);

		t->nombre = strdup(data);
		break;
	case TAREA_ATTR_DESCRIPCION:
		if (t->descripcion)
                        xfree(t->descripcion);

                t->descripcion = strdup(data);
                break;
	case TAREA_ATTR_ID:
		t->id = *((uint32_t *)data);
		break;
	case TAREA_ATTR_USUARIO:
		if (t->usuario)
                        xfree(t->usuario);

                t->usuario = strdup(data);
                break;
	case TAREA_ATTR_PRIORIDAD:
		t->prioridad = *((uint16_t *)data);
		break;
	}

	t->flags |= (1 << attr);
}

void tarea_attr_set_u32(struct tarea *t, uint16_t attr, uint32_t data)
{
	tarea_attr_set_data(t, attr, &data, sizeof(uint32_t));
}

void tarea_attr_set_u16(struct tarea *t, uint16_t attr, uint16_t data)
{
	tarea_attr_set_data(t, attr, &data, sizeof(uint16_t));
}
void tarea_attr_set_str(struct tarea *t, uint16_t attr, const char *data)
{
	tarea_attr_set_data(t, attr, data, 0);
}

const void *tarea_attr_get_data(struct tarea *t, uint16_t attr)
{
	if (!(t->flags & (1 << attr)))
		return NULL;

	switch(attr) {
	case TAREA_ATTR_NOMBRE:
		return t->nombre;
	case TAREA_ATTR_DESCRIPCION:
		return t->descripcion;
	case TAREA_ATTR_ID:
		return &t->id;
	case TAREA_ATTR_USUARIO:
		return t->usuario;
	case TAREA_ATTR_PRIORIDAD:
		return &t->prioridad;
	}
	return NULL;
}

uint32_t tarea_attr_get_u32(struct tarea *t, uint16_t attr)
{
	const void * ret = tarea_attr_get_data(t, attr);
	return ret == NULL ? 0 : *((uint32_t *)ret);
}

uint16_t tarea_attr_get_u16(struct tarea *t, uint16_t attr)
{
	const void * ret = tarea_attr_get_data(t, attr);
	return ret == NULL ? 0 : *((uint16_t *)ret);
}

const char *tarea_attr_get_str(struct tarea *t, uint16_t attr)
{
	return tarea_attr_get_data(t, attr);
}

int tarea_snprintf(char *buf, size_t size, struct tarea *t)
{
	return snprintf(buf, size, "La tarea número %d está enviada por %s."
			"Se llama %s y la descripción es %s\n",
			t->id, t->usuario, t->nombre, t->descripcion);
}
