#include "../include/gestor_tarea.h"
#include <assert.h>

int main(void)
{
	struct tarea *t1, *t2;
	struct gestor_tarea *g;
	char info[5000];

	t1 = tarea_alloc();

	tarea_attr_set_str(t1, TAREA_ATTR_NOMBRE, "1 - Tarea");
        tarea_attr_set_str(t1, TAREA_ATTR_DESCRIPCION,
                                "Descripción del ejercicio 2 (tarea 1).");
        tarea_attr_set_u32(t1, TAREA_ATTR_ID, 1972);
        tarea_attr_set_str(t1, TAREA_ATTR_USUARIO,
                                "Guillermo de la Cruz Dorado");
        tarea_attr_set_u16(t1, TAREA_ATTR_PRIORIDAD, 10);

	t2 = tarea_alloc();

	tarea_attr_set_str(t2, TAREA_ATTR_NOMBRE, "2 - Tarea");
        tarea_attr_set_str(t2, TAREA_ATTR_DESCRIPCION,
                                "Prueba para el ejercicio 2 (tarea 2).");
        tarea_attr_set_u32(t2, TAREA_ATTR_ID, 1978);
        tarea_attr_set_str(t2, TAREA_ATTR_USUARIO,
                                "Guillermo de la Cruz Dorado");
        tarea_attr_set_u16(t2, TAREA_ATTR_PRIORIDAD, 0);

	g = gestor_tarea_alloc();

	gestor_tarea_attr_set_tarea(g, GESTOR_TAREA_ATTR_TAREAS, t1);
	gestor_tarea_attr_set_tarea(g, GESTOR_TAREA_ATTR_TAREAS, t2);

	gestor_tarea_snprintf(info, sizeof(info), g);
	printf("%s\n", info);

	gestor_tarea_attr_unset_tarea(g, 3);
	assert(gestor_tarea_attr_get_u16(g, GESTOR_TAREA_ATTR_NUMERO_TAREAS)
		== 2);

	gestor_tarea_attr_unset_tarea(g, 0);
	assert(gestor_tarea_attr_get_u16(g, GESTOR_TAREA_ATTR_NUMERO_TAREAS)
                == 1);

	gestor_tarea_snprintf(info, sizeof(info), g);
        printf("%s", info);

	gestor_tarea_attr_unset_tarea(g, 0);
	assert(gestor_tarea_attr_get_u16(g, GESTOR_TAREA_ATTR_NUMERO_TAREAS)
                == 0);
	assert(gestor_tarea_attr_get_tarea(g, GESTOR_TAREA_ATTR_TAREAS, 0)
		== NULL);

	gestor_tarea_free(g);

	return 0;
}
