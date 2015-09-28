#include "../include/concesionario.h"
#include <getopt.h>
#include <gtk/gtk.h>

void cerrar_window(GtkWidget *widget,gpointer data);
void append_coches(GtkTreeStore *store, struct concesionario *con);

static GtkWidget *mainwindow;
static struct concesionario *con;
int id = 0;
static char path[250] = "../files/";

//funcion que añade un coche a la lista y guarda en fichero
static void add_car(GtkButton *widget, gpointer data)
{
	GObject **widgs;
	struct coche *c;
	GtkWidget *entry_ptr_marca;
	GtkWidget *entry_ptr_matricula;
	GtkTreeStore *store;
	GtkTreeIter iter;
	const gchar *matricula, *marca;

	widgs = (GObject **)data;
	entry_ptr_marca = (GtkWidget *)widgs[1];
	entry_ptr_matricula = (GtkWidget *)widgs[2];
	store = (GtkTreeStore *)widgs[3];

	marca = gtk_entry_get_text((GtkEntry *)entry_ptr_marca);
	matricula = gtk_entry_get_text((GtkEntry *)entry_ptr_matricula);

	c = curso_coche_alloc();
	if (c == NULL)
		return;

	curso_coche_attr_set_u32(c, CURSO_COCHE_ATTR_ID, id++);
	curso_coche_attr_set_str(c, CURSO_COCHE_ATTR_MATRICULA,	matricula);
	curso_coche_attr_set_str(c, CURSO_COCHE_ATTR_MARCA, marca);

	curso_concesionario_attr_set_coche(con, CURSO_CONCESIONARIO_ATTR_COCHE,
					   c);

	gtk_tree_store_append (store, &iter, NULL);
        gtk_tree_store_set (store, &iter,
                        0, FALSE,
                        1, id - 1,
                        2, marca,
                        3, matricula, -1);

	write_concesionario_to_file(path, con);

	gtk_widget_destroy((GtkWidget *) widgs[0]);
}

//funcion que cambia el dueño del concesionario, ademas de en la aplicacion
static void change_dueno(GtkButton *widget, gpointer data)
{
	GtkWidget *entry;
	GtkWidget *labeldueno;
	GtkWidget **widgs;
	gchar dueno[50];

	widgs = (GtkWidget **)data;
	entry = widgs[1];
	labeldueno = widgs[2];
	strncpy(dueno, gtk_entry_get_text((GtkEntry *)entry), 50);

	curso_concesionario_attr_set_str(con, CURSO_CONCESIONARIO_ATTR_DUENO,
					dueno);

	gtk_label_set_text((GtkLabel *)labeldueno, dueno);
	strcpy(path, "../files/");
	strcat(path, dueno);
	strcat(path, ".conc");
	gtk_window_set_title((GtkWindow *)mainwindow, path);

	gtk_widget_destroy((GtkWidget *)widgs[0]);
}

//funcion que elimina los coches seleccionados de la lista y del concesionario
static void remove_coches(GtkButton *widget, gpointer data)
{
	GObject **widgs;
	GtkTreeStore *store;
	GtkTreeIter iter;
	GtkTreeIter iterauxiliar;
	gboolean isIter;
	gboolean enabled;
	int pos = 0;
	widgs = (GObject **)data;
	store = (GtkTreeStore *)widgs[1];

	isIter = gtk_tree_model_get_iter_first(GTK_TREE_MODEL (widgs[1]),
						&iter);

	while (isIter) {
		iterauxiliar = iter;
		isIter = gtk_tree_model_iter_next(
				GTK_TREE_MODEL (widgs[1]), &iter);

		gtk_tree_model_get(GTK_TREE_MODEL (store), &iterauxiliar, 0,
					&enabled, -1);
		if (enabled) {
			gtk_tree_store_remove(store, &iterauxiliar);
			curso_concesionario_attr_unset_coche(con,  pos);
		}
		pos++;
	}

	write_concesionario_to_file(path, con);

	gtk_widget_destroy((GtkWidget *)widgs[0]);
}

//funcion de señal para la ventaña de añadir
//llama a la funcion add_car
static void show_add_window(GtkButton *widget, gpointer data)
{
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *buttoncancel;
	GtkWidget *entry1, *entry2;
	GtkWidget *box;
	GtkWidget *labelmatricula;
        GtkWidget *labelmarca;
	GObject **widgs;

	widgs = calloc(1, 4 * sizeof(GObject));

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Añada nuevo coche");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 360, 100);
	labelmatricula = gtk_label_new("Matricula:");
	labelmarca = gtk_label_new("Marca:");
	button = gtk_button_new_with_label("Añade");
	buttoncancel = gtk_button_new_with_label("Cancelar");
	box = gtk_vbox_new(TRUE,2);
	entry1 = gtk_entry_new();
	entry2 = gtk_entry_new();

	widgs[0] = (GObject *)window;
	widgs[1] = (GObject *)entry2;
	widgs[2] = (GObject *)entry1;
	widgs[3] = (GObject *)data;

	gtk_box_pack_start ((GtkBox *)box, labelmarca, FALSE, TRUE, 2);
	gtk_box_pack_start ((GtkBox *)box, entry1, TRUE, TRUE, 2);
	gtk_box_pack_start ((GtkBox *)box, labelmatricula, FALSE, FALSE, 2);
	gtk_box_pack_start ((GtkBox *)box, entry2, TRUE, TRUE, 2);

	gtk_box_pack_start ((GtkBox *)box, button, TRUE, TRUE, 10);
	gtk_box_pack_start ((GtkBox *)box, buttoncancel, TRUE, TRUE, 10);

	gtk_container_add((GtkContainer *)window, box);

	g_signal_connect((GObject *)button, "clicked", (GCallback)add_car,
			 (gpointer)widgs);
	g_signal_connect(G_OBJECT(buttoncancel), "clicked",
			G_CALLBACK(cerrar_window), (gpointer)window); 

	gtk_widget_show_all(window);
}

//funcion de señal para cambiar el dueño del concesionario
//llama a la funcion change_dueno anterior
static void show_change_dueno(GtkButton *widget, gpointer data)
{
        GtkWidget *window;
        GtkWidget *label;
        GtkWidget *buttonok;
        GtkWidget *buttoncancel;
        GtkWidget *vbox;
        GtkWidget *entrydueno;
	GtkWidget **widgs;

	widgs = calloc(1, 3 * sizeof(GtkWidget));

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Cambia de dueño");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
        buttonok = gtk_button_new_with_label("Ok");
        buttoncancel = gtk_button_new_with_label("Cancel");
        vbox = gtk_vbox_new(TRUE, 2);
        entrydueno = gtk_entry_new();
	label = gtk_label_new("Nuevo dueño:");

	widgs[0] = window;
        widgs[1] = entrydueno;
	widgs[2] = (GtkWidget *)data;

        gtk_box_pack_start (GTK_BOX(vbox), label, TRUE, TRUE, 2);
        gtk_box_pack_start (GTK_BOX(vbox), entrydueno, TRUE, TRUE, 2);
        gtk_box_pack_start (GTK_BOX(vbox), buttonok, TRUE, TRUE, 2);
        gtk_box_pack_start (GTK_BOX(vbox), buttoncancel, TRUE, TRUE, 2);

        gtk_container_add((GtkContainer *)window, vbox);

        g_signal_connect((GObject *)buttonok, "clicked", G_CALLBACK(change_dueno),
			(gpointer)widgs);
        g_signal_connect(G_OBJECT(buttoncancel), "clicked",
                      G_CALLBACK(cerrar_window),(gpointer)window);

        gtk_widget_show_all(window);
}

//funcion de señal que lanza una ventana de seguridad
//lanza la funcion remove_coches anterior
static void show_remove_coches(GtkButton *widget, gpointer data)
{
	GtkWidget *window;
	GtkWidget *label;
	GtkWidget *buttony;
	GtkWidget *buttonn;
	GtkWidget *vbox;
	GObject **widgs;

	widgs = calloc(1, 2 * sizeof(GObject));

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Borrar seleccionados");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
	buttony = gtk_button_new_with_label("Si");
	buttonn = gtk_button_new_with_label("No");
	label = gtk_label_new("¿Borrar seleccionados?");
	vbox = gtk_vbox_new(TRUE, 2);

	widgs[0] = (GObject *)window;
	widgs[1] = (GObject *)data;

	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox), buttony, TRUE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox), buttonn, TRUE, TRUE, 2);

	gtk_container_add((GtkContainer *)window, vbox);

	g_signal_connect(G_OBJECT(buttony), "clicked",
			G_CALLBACK(remove_coches), (gpointer)widgs);
	g_signal_connect(G_OBJECT(buttonn), "clicked",
			G_CALLBACK(cerrar_window), (gpointer)window);

	gtk_widget_show_all(window);
}

//funcion de señal para cambiar el checkbox de los elementos de la lista
//cuando se pulsa la celda, se lanza esta funcion y cambia su valor booleano
static void buttonToggled(GtkCellRendererToggle* renderer, gchar* pathStr, gpointer data)
{
	GtkTreeIter iter;
	gboolean enabled;

	GtkTreePath* path = gtk_tree_path_new_from_string(pathStr);
	gtk_tree_model_get_iter(GTK_TREE_MODEL (data), &iter, path);
	gtk_tree_model_get(GTK_TREE_MODEL (data), &iter, 0, &enabled, -1);
	enabled = enabled?0:1;
	gtk_tree_store_set(GTK_TREE_STORE (data), &iter, 0, enabled, -1);
}

//funcion para cerrar la ventana principal
static void funcion_quit(GtkButton *boton, gpointer data)
{
	curso_concesionario_free(con);
	gtk_main_quit();
}

//funcion para cerrar las demas ventanas que no sean la principal
void cerrar_window (GtkWidget *widget,gpointer data)
{
        gtk_widget_destroy((GtkWidget *)data);
}

//funcion para rellenar la lista con los coches del concesionario
void append_coches(GtkTreeStore *store, struct concesionario *con)
{
	GtkTreeIter iter;
	struct coche *c;
	int i;
	int numcoches = curso_concesionario_attr_get_u32(con,
			CURSO_CONCESIONARIO_ATTR_NUM_COCHES);

	for (i = 0; i < numcoches; i++) {
		c = curso_concesionario_attr_get_coche(con,
			CURSO_CONCESIONARIO_ATTR_COCHE, i);

		gtk_tree_store_append (store, &iter, NULL);
		gtk_tree_store_set (store, &iter,
                        0, FALSE,
                        1, c->id,
                        2, c->marca,
                        3, c->matricula, -1);
	}

	id = c->id + 1;
}

int main (int argc, char *argv[])
{
	char d[50];
	int valor, option_index = 0;
	uint8_t flags = 0;

	GtkWidget *button1;
	GtkWidget *buttondueno;
	GtkWidget *buttonremove;
	GtkWidget *vbox;
	GtkWidget *vbox2;
	GtkWidget *vbox3;
	GtkWidget *hbox;
	GtkWidget *hbox2;
	GtkWidget *hbox3;

	GtkTreeStore *store;
	GtkWidget *treeview;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	GtkTreeIter *iter;

	GtkWidget *menubar;
	GtkWidget *file;
	GtkWidget *edit;
	GtkWidget *filemenu;
	GtkWidget *editmenu;
	GtkWidget *cambiardueno;
	GtkWidget *anadecoche;
	GtkWidget *removecoches;
	GtkWidget *quit;

	GtkWidget *labeldueno;
	GtkWidget *labeldueno2;
	GtkWidget *labellista;

	//se debe indicar con la bandera el nombre del dueño solo, sin .conc
	//por ejemplo: aplicacion -c Guille
	static struct option long_options[] = {
		{ "concesionario", required_argument, 0, 'c'},
		{ "help", 0, 0, 'h'},
		{ 0 }
	};

	while((valor = getopt_long(argc, argv, "c:h", long_options,
				&option_index)) != -1) {

		switch(valor) {
		case 'c':
			strncpy(d, optarg, 50);
			flags |= 1 << 0;
			break;
		case 'h':
		default:
			flags |= 1 << 1;
			printf("Parametro de entrada: -c/-concesionario "
				"{nombre dueño}\n");
			break;
		}
	}

	if (flags > 1)
		return 0;


	if (flags != 1)
		strcpy(d, "NombrePorDefecto");

	strcat(path, d);
	strcat(path, ".conc");

	con = curso_concesionario_alloc();

	if (con == NULL)
			return -1;

	curso_concesionario_attr_set_str(con, CURSO_CONCESIONARIO_ATTR_DUENO,
					 d);

	//si se ha indicado la bandera -c se lee del archivo si existe
	if (flags == 1)
		read_file_to_concesionario(path, con);


	gtk_init(&argc, &argv);

	strcpy(d, curso_concesionario_attr_get_str(con,
		CURSO_CONCESIONARIO_ATTR_DUENO));

	//inicializando widgets
	mainwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title((GtkWindow *)mainwindow, path);
	gtk_window_set_position(GTK_WINDOW(mainwindow), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(mainwindow), 420, 420);
	button1 = gtk_button_new_with_label("Añadir Coche");
	buttondueno = gtk_button_new_with_label("Cambia dueño");
	buttonremove = gtk_button_new_with_label("Borrar");
	vbox = gtk_vbox_new(FALSE, 3);
	vbox2 = gtk_vbox_new(FALSE, 0);
	vbox3 = gtk_vbox_new(FALSE, 1);
	hbox = gtk_hbox_new(FALSE, 2);
	hbox2 = gtk_hbox_new(FALSE, 3);
	hbox3 = gtk_hbox_new(FALSE, 5);
	menubar = gtk_menu_bar_new();
	filemenu = gtk_menu_new();
	editmenu = gtk_menu_new();
	file = gtk_menu_item_new_with_label("File");
	edit = gtk_menu_item_new_with_label("Edit");
	cambiardueno = gtk_menu_item_new_with_label("Cambiar dueño ...");
	anadecoche = gtk_menu_item_new_with_label("Añade coche ...");
	removecoches = gtk_menu_item_new_with_label("Borrar coches ...");
	quit = gtk_menu_item_new_with_label("Salir");

	//creando la barra de menu
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), editmenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), cambiardueno);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), anadecoche);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), removecoches);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), edit);
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 3);

	//agrupando boxes
	labeldueno = gtk_label_new("Dueño: ");
	labeldueno2 = gtk_label_new(d);
	labellista = gtk_label_new("Lista de coches:");
	gtk_box_pack_start(GTK_BOX(hbox), labeldueno, TRUE, FALSE, 6);
	gtk_box_pack_start(GTK_BOX(hbox), labeldueno2, FALSE, FALSE, 8);
	gtk_box_pack_start(GTK_BOX(hbox), buttondueno, TRUE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(hbox2), labellista ,FALSE, FALSE, 20);

	gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox2, FALSE, FALSE, 5);

	//generando la vista de la lista de coches
	treeview = gtk_tree_view_new();
	store = gtk_tree_store_new(4, G_TYPE_BOOLEAN, G_TYPE_INT,
					 G_TYPE_STRING,	G_TYPE_STRING);

	treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
	g_object_unref (G_OBJECT (store));

	renderer = gtk_cell_renderer_toggle_new ();
	column = gtk_tree_view_column_new_with_attributes ("Check",
				renderer, "active", 0, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);
	g_object_set(renderer, "activatable", TRUE, NULL);
	//señal que lanza la funcion buttonToggled de arriba
	g_signal_connect(renderer, "toggled", (GCallback) buttonToggled,
				store);

	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Id",
				renderer, "text", 1, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

	renderer = gtk_cell_renderer_text_new ();
        column = gtk_tree_view_column_new_with_attributes ("Matricula",
                                renderer, "text", 2, NULL);
        gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Marca",
				renderer, "text", 3, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

	append_coches(store, con);

	//agrupando mas boxes
	gtk_box_pack_start(GTK_BOX(hbox3), treeview, TRUE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox3), button1, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox3), buttonremove, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox3), vbox3, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox3, TRUE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox), vbox2, TRUE, TRUE, 2);
	gtk_container_add((GtkContainer *)mainwindow, vbox);

	//señales, en algunas se pasan los punteros a widgets a modificar
	/*los items de la barra de menu lanzan las mismas funciones que los
	  botones, pasando los mismos datos */
	g_signal_connect(G_OBJECT(button1), "clicked",
			G_CALLBACK(show_add_window), store);
	g_signal_connect(G_OBJECT(mainwindow), "delete-event",
			G_CALLBACK(funcion_quit), NULL);
	g_signal_connect(G_OBJECT(buttondueno), "clicked",
			G_CALLBACK(show_change_dueno), labeldueno2);
	g_signal_connect(G_OBJECT(buttonremove), "clicked",
			G_CALLBACK(show_remove_coches), store);
	g_signal_connect(G_OBJECT(anadecoche), "activate",
			G_CALLBACK(show_add_window), store);
	g_signal_connect(G_OBJECT(removecoches), "activate",
			G_CALLBACK(show_remove_coches), store);
	g_signal_connect(G_OBJECT(cambiardueno), "activate",
			G_CALLBACK(show_change_dueno), labeldueno2);
	g_signal_connect(G_OBJECT(quit), "activate",
                        G_CALLBACK(funcion_quit), NULL);

	gtk_widget_show_all(mainwindow);

	gtk_main();
	return 0;
}
