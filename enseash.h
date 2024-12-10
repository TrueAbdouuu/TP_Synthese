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
#define INPUT_BUFFER_SIZE 128
#define EXIT_MESSAGE "Bye bye...\n"
#define STATUS_SIZE 50

// Function Prototypes
void display_welcome_message();
void display_prompt();
int read_input(char *input);

int execute_command(char *command, long *elapsed_time_ms) ;


void get_command_status(int status,long elapsed_time_ms, char *status_string, size_t size);

#endif // ENSEASH_H
