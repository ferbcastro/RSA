// Include gtk
#include <gtk/gtk.h>

// Callback para criptografar
void on_encrypt_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **) data;
    const char *input = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    encrypt(input);
    gtk_entry_set_text(GTK_ENTRY(entries[1]), "Criptografado com sucesso!");
}

// Callback para descriptografar
void on_decrypt_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **) data;
    char output[256];
    decrypt(output);
    gtk_entry_set_text(GTK_ENTRY(entries[1]), output);
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *grid, *entry_input, *entry_output, *encrypt_button, *decrypt_button;

    gtk_init(&argc, &argv);

    // Cria a janela
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Criptografia");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Layout em grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Campo de entrada para o texto a ser criptografado
    entry_input = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_input), "Digite o texto aqui");
    gtk_grid_attach(GTK_GRID(grid), entry_input, 0, 0, 2, 1);

    // Campo de saída para o resultado da descriptografia
    entry_output = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_output), "Resultado aparecerá aqui");
    gtk_grid_attach(GTK_GRID(grid), entry_output, 0, 1, 2, 1);

    // Botão de criptografia
    encrypt_button = gtk_button_new_with_label("Criptografar");
    gtk_grid_attach(GTK_GRID(grid), encrypt_button, 0, 2, 1, 1);

    // Botão de descriptografia
    decrypt_button = gtk_button_new_with_label("Descriptografar");
    gtk_grid_attach(GTK_GRID(grid), decrypt_button, 1, 2, 1, 1);

    // Array para passar os widgets aos callbacks
    GtkWidget *entries[] = { entry_input, entry_output };

    // Conectar os botões aos callbacks
    g_signal_connect(encrypt_button, "clicked", G_CALLBACK(on_encrypt_button_clicked), entries);
    g_signal_connect(decrypt_button, "clicked", G_CALLBACK(on_decrypt_button_clicked), entries);

    // Exibir tudo
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}