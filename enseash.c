#include "enseash.h"  // Inclure l'en-tête pour les déclarations de fonctions et constantes

#include <stdio.h>
#include <string.h>  // Pour strlen()

// Affiche le message de bienvenue
void display_welcome_message() {
    int terminal = STDOUT_FILENO;
    write(terminal, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
    close(terminal);  // Ferme le descripteur après usage
}

// Affiche le prompt
void display_prompt() {
    int terminal = STDOUT_FILENO;
    write(terminal, PROMPT, strlen(PROMPT));
    close(terminal);  // Ferme le descripteur après usage
}

// Lit l'entrée utilisateur
int read_input(char *input) {
    ssize_t bytesRead = read(STDIN_FILENO, input, INPUT_BUFFER_SIZE - 1);
    if (bytesRead > 0) {
        input[bytesRead - 1] = '\0';  // Retirer le caractère de nouvelle ligne
    }
    return bytesRead;
}

// Exécute la commande entrée par l'utilisateur
void execute_command(const char *input) {
    pid_t pid = fork();
    int status;
    struct timespec start, end;

    if (pid == 0) {
        // Processus fils : exécute la commande
        clock_gettime(CLOCK_MONOTONIC, &start);  // Commence à mesurer le temps
        execlp(input, input, NULL);
        perror("Command execution failed");  // Si execlp échoue
        _exit(1);
    } else if (pid > 0) {
        // Processus parent : attend que le processus fils termine
        waitpid(pid, &status, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);  // Fin de la mesure du temps

        // Calcul du temps d'exécution
        long seconds = end.tv_sec - start.tv_sec;
        long nanoseconds = end.tv_nsec - start.tv_nsec;
        double execution_time = seconds * 1000.0 + nanoseconds / 1000000.0;

        // Affichage du code de retour et du temps d'exécution
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            char exit_message[100];
            sprintf(exit_message, "enseash [exit:%d|%.2fms] %% ", exit_code, execution_time);
            write(STDOUT_FILENO, exit_message, strlen(exit_message));
        } else if (WIFSIGNALED(status)) {
            int signal_code = WTERMSIG(status);
            char signal_message[100];
            sprintf(signal_message, "enseash [sign:%d|%.2fms] %% ", signal_code, execution_time);
            write(STDOUT_FILENO, signal_message, strlen(signal_message));
        }
    } else {
        perror("Fork failed");
    }
}
