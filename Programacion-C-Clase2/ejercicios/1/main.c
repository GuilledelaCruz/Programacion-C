#include "tarea.h"
#include <assert.h>

int main(void)
{
        struct tarea *t;
        char info[250];

        t = tarea_alloc();

        tarea_attr_set_str(t, TAREA_ATTR_NOMBRE, "1 - Tarea");
	assert(strcmp(tarea_attr_get_str(t, TAREA_ATTR_NOMBRE),
			"1 - Tarea") == 0);
        tarea_attr_set_str(t, TAREA_ATTR_DESCRIPCION, 
				"Descripción del ejercicio 1 (tarea).");
	assert(strcmp(tarea_attr_get_str(t, TAREA_ATTR_DESCRIPCION),
                        "Descripción del ejercicio 1 (tarea).") == 0);
        tarea_attr_set_u32(t, TAREA_ATTR_ID, 1972);
	assert(tarea_attr_get_u32(t, TAREA_ATTR_ID) == 1972);
	tarea_attr_set_str(t, TAREA_ATTR_USUARIO,
				"Guillermo de la Cruz Dorado");
	assert(strcmp(tarea_attr_get_str(t, TAREA_ATTR_USUARIO),
                        "Guillermo de la Cruz Dorado") == 0);
	tarea_attr_set_u16(t, TAREA_ATTR_PRIORIDAD, 0);
	assert(tarea_attr_get_u16(t, TAREA_ATTR_PRIORIDAD) == 0);

        tarea_snprintf(info, sizeof(info), t);
        printf("los valores de la tarea son: %s\n", info);

        tarea_free(t);
        return 0;
}

