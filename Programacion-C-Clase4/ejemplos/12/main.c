#include "concesionario.h"
#include <gtk/gtk.h>

struct concesionario *con;
int id;

static void add_car(GtkButton *widget, gpointer data)
{
	GtkWidget **entry;
	struct coche *c;
	GtkWidget *entry_ptr_marca;
	GtkWidget *entry_ptr_matricula;
	const gchar *matricula, *marca;

	entry = (GtkWidget **)data;
	entry_ptr_marca = entry[0];
	entry_ptr_matricula = entry[1];

	printf("llega\n");

	marca = gtk_entry_get_text((GtkEntry *)entry_ptr_marca);
	matricula = gtk_entry_get_text((GtkEntry *)entry_ptr_matricula);

	c = curso_coche_alloc();
	if (c == NULL)
		return;

	curso_coche_attr_set_u32(c, CURSO_COCHE_ATTR_ID, id++);
	curso_coche_attr_set_str(c, CURSO_COCHE_ATTR_MATRICULA, matricula);
	curso_coche_attr_set_str(c, CURSO_COCHE_ATTR_MARCA, marca);

	curso_concesionario_attr_set_coche(con, CURSO_CONCESIONARIO_ATTR_COCHE,
					   c);

	printf ("Result: %d , %s, %s\n", id - 1, marca, matricula);
}

static void show_add_window(GtkButton *widget, gpointer data)
{
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *entry1, *entry2;
	GtkWidget *box;
	GtkWidget **entry;

	entry = calloc(1, 2 * sizeof(GtkWidget));

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	button = gtk_button_new_with_label("Add me");
	box = gtk_vbox_new(TRUE,2);
	entry1 = gtk_entry_new();
	entry2 = gtk_entry_new();

	entry[0] = entry1;
	entry[1] = entry2;

	gtk_box_pack_start ((GtkBox *)box, entry1, TRUE, TRUE, 2);
	gtk_box_pack_start ((GtkBox *)box, entry2, TRUE, TRUE, 2);

	gtk_box_pack_start ((GtkBox *)box, button, TRUE, TRUE, 2);

	gtk_container_add((GtkContainer *)window, box);

	g_signal_connect((GObject *)button, "clicked", (GCallback)add_car,
			 (gpointer)entry);

	gtk_widget_show_all(window);
}

static void funcion_quit(GtkButton *boton, gpointer data)
{
	curso_concesionario_free(con);
	gtk_main_quit();
}

int main (int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *button1;
	GtkWidget *button2;
	GtkWidget *vbox;

	con = curso_concesionario_alloc();
	if (con == NULL)
		return -1;

	curso_concesionario_attr_set_str(con, CURSO_CONCESIONARIO_ATTR_DUENO,
					 "Alvaro");

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	button1 = gtk_button_new_with_label("Añadir Coche");
	vbox = gtk_vbox_new(TRUE,5);

	gtk_window_set_title((GtkWindow *)window, "Concesionario");

	gtk_box_pack_start((GtkBox *)vbox, button1, TRUE, TRUE, 2);

	gtk_container_add((GtkContainer *)window, vbox);

	g_signal_connect((GObject *)button1, "clicked",
			 (GCallback)show_add_window, NULL);

	g_signal_connect((GObject *)window, "delete-event",
			 (GCallback)funcion_quit, NULL);

	gtk_widget_show_all(window);

	gtk_main();
	return 0;
}
