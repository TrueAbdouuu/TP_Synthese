//
// Created by abdouuu on 03/12/24.
//

#ifndef ENSEASH_H
#define ENSEASH_H

#include <sys/types.h>  // Pour pid_t
#include <unistd.h>      // Pour write(), read(), execlp()
#include <time.h>        // Pour clock_gettime()
#include <sys/wait.h>    // Pour waitpid()

// Constants
#define WELCOME_MESSAGE "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n"
#define PROMPT "enseash % "
#define GOODBYE_MESSAGE "Bye bye...\n"
#define INPUT_BUFFER_SIZE 128

// Function Prototypes
void display_welcome_message();  // Fonction pour afficher le message de bienvenue
void display_prompt();            // Fonction pour afficher le prompt
int read_input(char *input);      // Fonction pour lire l'entrée de l'utilisateur
void execute_command(const char *input);  // Fonction pour exécuter la commande et afficher le code de retour et le temps d'exécution

#endif // ENSEASH_H
