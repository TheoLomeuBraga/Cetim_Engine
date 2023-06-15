#include "font_reader.h"

#include <gtk/gtk.h>

#include <fstream>

bool file_exists(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    return file.good();
}

GtkBuilder *builder, *builder_popup;

void close_window(GtkButton *button, gpointer user_data)
{
    gtk_widget_destroy((GtkWidget *)user_data);
}

void error_popup(std::string error)
{

    builder_popup = gtk_builder_new_from_file("font_reader_gtk.glade");
    // GtkWidget *button_show_popup = GTK_WIDGET(gtk_builder_get_object(builder_popup, "button_show_popup"));

    // Obtem um ponteiro para a janela do popup
    GtkWidget *popup = GTK_WIDGET(gtk_builder_get_object(builder_popup, "error_popup"));

    GtkWidget *popup_label = GTK_WIDGET(gtk_builder_get_object(builder_popup, "error_mensage"));
    gtk_label_set_text(GTK_LABEL(popup_label), error.c_str());

    // Conecta o sinal "clicked" do botão "Fechar" à função de callback "on_popup_close_clicked"
    GtkWidget *button_close_popup = GTK_WIDGET(gtk_builder_get_object(builder_popup, "ok"));
    g_signal_connect(button_close_popup, "clicked", G_CALLBACK(close_window), popup);

    // Exibe o popup
    gtk_widget_show_all(popup);

    g_object_unref(builder_popup);
}

std::string fontFilePath = "";
std::string jsonFilePath = "";
std::string jsonFileName = "font.json";
bool pixel_perfect = true;
int quality = 40;

void apply(GtkButton *apply_button, gpointer user_data)
{
    bool fontFilePathExist = file_exists(fontFilePath.c_str()), qualityAboveZero = quality > 0;

    std::cout << jsonFilePath + std::string("/") + jsonFileName << std::endl;

    if (fontFilePathExist && qualityAboveZero)
    {
        storeCharsInfoInJSON(fontFilePath.c_str(), (jsonFilePath + std::string("/") + jsonFileName).c_str(), pixel_perfect, quality);
    }
    else
    {
        if (fontFilePathExist == false)
        {
            error_popup("\nerror path not exist\n");
        }
        if (qualityAboveZero == false)
        {
            error_popup("\nerror quality should be biger than zero\n");
        }
    }
}

void set_fontFilePath(GtkFileChooser *text_input, gpointer user_data)
{
    fontFilePath = gtk_file_chooser_get_preview_filename(text_input);
    std::cout << fontFilePath << std::endl;
}

void set_jsonFilePath(GtkFileChooser *text_input, gpointer user_data)
{
    jsonFilePath = gtk_file_chooser_get_filename(text_input);
    std::cout << jsonFilePath << std::endl;
}

void set_jsonFileName(GtkEntry *text_input, gpointer user_data)
{
    jsonFileName = gtk_entry_get_text(text_input);
    std::cout << jsonFileName << std::endl;
}

void set_quality(GtkEntry *text_input, gpointer user_data)
{
    quality = atof(gtk_entry_get_text(text_input));
    std::cout << quality << std::endl;
}

void set_pixel_perfect(GtkWidget *widget, gpointer data)
{
    pixel_perfect = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
    std::cout << pixel_perfect << std::endl;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // Criando o builder a partir do arquivo Glade
    builder = gtk_builder_new_from_file("font_reader_gtk.glade");

    // Obtendo um objeto GtkWidget do builder
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    if (file_exists("theme.css"))
    {
        GtkCssProvider *provider = gtk_css_provider_new();
        GdkDisplay *display = gdk_display_get_default();
        GdkScreen *screen = gdk_display_get_default_screen(display);
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        gtk_css_provider_load_from_path(provider, "theme.css", NULL);

        //gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
        
    }

    GtkWidget *font_file_input_fild = GTK_WIDGET(gtk_builder_get_object(builder, "font_file"));
    g_signal_connect(font_file_input_fild, "file-set", G_CALLBACK(set_fontFilePath), NULL);

    GtkWidget *output_folder_input_fild = GTK_WIDGET(gtk_builder_get_object(builder, "output_folder"));
    g_signal_connect(output_folder_input_fild, "file-set", G_CALLBACK(set_jsonFilePath), NULL);

    GtkWidget *file_name_input_fild = GTK_WIDGET(gtk_builder_get_object(builder, "file_name"));
    g_signal_connect(file_name_input_fild, "changed", G_CALLBACK(set_jsonFileName), NULL);

    GtkWidget *set_quality_fild = GTK_WIDGET(gtk_builder_get_object(builder, "quality"));
    g_signal_connect(set_quality_fild, "changed", G_CALLBACK(set_quality), NULL);

    GtkWidget *pixel_perfect_fild = GTK_WIDGET(gtk_builder_get_object(builder, "pixel_perfect"));
    g_signal_connect(pixel_perfect_fild, "toggled", G_CALLBACK(set_pixel_perfect), NULL);

    GtkWidget *apply_button = GTK_WIDGET(gtk_builder_get_object(builder, "apply"));
    g_signal_connect(apply_button, "clicked", G_CALLBACK(apply), NULL);

    // Mostrando a janela
    gtk_widget_show_all(window);

    gtk_main();

    // Liberando a memória alocada pelo builder
    g_object_unref(builder);

    return 0;
}
