#include <locale.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "encrypt.h"
#include "decrypt.h"

// Callback para criptografar
void on_encrypt_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **) data;
    const char *input_n = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const char *input_d = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const char *input_text = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    char *str = encrypt(input_text, input_n, input_d);
    gtk_entry_set_text(GTK_ENTRY(entries[3]), "");
    gtk_entry_set_text(GTK_ENTRY(entries[4]), str);
    free(str);
}

// Callback para descriptografar
void on_decrypt_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **) data;
    const char *input_n = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const char *input_e = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    char* str = decrypt(input_n, input_e);
    gtk_entry_set_text(GTK_ENTRY(entries[4]), "");
    gtk_entry_set_text(GTK_ENTRY(entries[3]), str);
    free(str);
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *grid, *text_input, *text_output, *encrypt_button, *decrypt_button, 
              *value_n_input, *value_d_input, *value_e_input;
    GtkStyleContext *context;
    GtkCssProvider *css_provider;
    
    setlocale(LC_ALL, "pt_BR.UTF-8");
    gtk_init(&argc, &argv);
    css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
    "* { font-family: RobotoMono; font-size: 17px; }", -1, NULL);

    // Cria a janela
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Criptografia");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);
    context = gtk_widget_get_style_context(GTK_WIDGET(window));
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Layout em grid
    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Campo de entrada do valor 'n'
    value_n_input = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(value_n_input), "Valor de 'n'");
    gtk_grid_attach(GTK_GRID(grid), value_n_input, 0, 0, 1, 1);

    // Campo de entrada do valor 'd'
    value_d_input = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(value_d_input), "Valor de 'd'");
    gtk_grid_attach(GTK_GRID(grid), value_d_input, 0, 1, 1, 1);

    // Campo de entrada do valor 'e'
    value_e_input = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(value_e_input), "Valor de 'e'");
    gtk_grid_attach(GTK_GRID(grid), value_e_input, 0, 2, 1, 1);

    // Campo de entrada para o texto a ser criptografado
    text_input = gtk_entry_new();
    gtk_widget_set_margin_top(text_input, 10);
    gtk_entry_set_placeholder_text(GTK_ENTRY(text_input), "Texto puro");
    gtk_grid_attach(GTK_GRID(grid), text_input, 0, 3, 4, 1);

    // Campo de saída para o resultado da criptografia
    text_output = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(text_output), "Texto criptografado");
    gtk_grid_attach(GTK_GRID(grid), text_output, 0, 4, 4, 1);

    // Botão de criptografia
    encrypt_button = gtk_button_new_with_label("Criptografar");
    gtk_grid_attach(GTK_GRID(grid), encrypt_button, 0, 5, 4, 1);

    decrypt_button = gtk_button_new_with_label("Descriptografar");
    gtk_grid_attach(GTK_GRID(grid), decrypt_button, 0, 6, 4, 1);

    // Array para passar os widgets aos callbacks
    GtkWidget *entries[] = { value_n_input, value_d_input, value_e_input, text_input, text_output };

    // Conectar os botões aos callbacks
    g_signal_connect(encrypt_button, "clicked", G_CALLBACK(on_encrypt_button_clicked), entries);
    g_signal_connect(decrypt_button, "clicked", G_CALLBACK(on_decrypt_button_clicked), entries);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}