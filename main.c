#include <stdio.h>
#include <string.h>
#include "enseash.h"  // Inclure l'en-tête pour utiliser les fonctions de enseash.c

int main() {
    char input[INPUT_BUFFER_SIZE]; // Buffer pour l'entrée utilisateur
    ssize_t bytesRead;

    // Affiche le message de bienvenue
    display_welcome_message();

    while (1) {
        // Affiche le prompt et lit l'entrée utilisateur
        display_prompt();
        bytesRead = read_input(input);

        // Si l'utilisateur tape "exit" ou appuie sur Ctrl+D (EOF), on quitte
        if (bytesRead == 0 || (bytesRead > 0 && strncmp(input, "exit", 4) == 0)) {
            write(STDOUT_FILENO, GOODBYE_MESSAGE, strlen(GOODBYE_MESSAGE));
            break;  // Quitte la boucle et termine le programme
        }

        // Exécute la commande entrée par l'utilisateur
        execute_command(input);
    }

    return 0;
}
