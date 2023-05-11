#include "font_reader.h"

#include <gtk/gtk.h>

#include <fstream>

bool file_exists(const std::string& filename) {
    std::ifstream file(filename.c_str());
    return file.good();
}

const char* fontFilePath = ""; 
const char* jsonFilePath = "";
bool pixel_perfect = false;
int quality = 40;

void apply_button_clicked_cb(GtkButton *apply_button, gpointer user_data){
    if(file_exists(fontFilePath)){
        storeCharsInfoInJSON(fontFilePath,  jsonFilePath, pixel_perfect, quality);
    }else{
        std::cout << "error path not exist\n";
    }
}

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    // Criando o builder a partir do arquivo Glade
    GtkBuilder* builder = gtk_builder_new_from_file("font_reader_gtk.glade");

    // Obtendo um objeto GtkWidget do builder
    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    //GtkWidget* apply_button = GTK_WIDGET(gtk_builder_get_object(builder, "apply_button"));
    //g_signal_connect(apply_button, "clicked", G_CALLBACK(apply_button_clicked_cb), NULL);


    // Mostrando a janela
    gtk_widget_show_all(window);
    

    gtk_main();

    // Liberando a memória alocada pelo builder
    g_object_unref(builder);

    return 0;
}

