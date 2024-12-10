#include "enseash.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define EXIT_MESSAGE "Bye bye...\n"

void display_welcome_message() {
    int terminal = STDOUT_FILENO;
    write(terminal, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
}

void display_prompt(char *status_string) {
    int terminal = STDOUT_FILENO;
    char prompt_message[200];
    snprintf(prompt_message, sizeof(prompt_message), "enseash %s ", status_string);
    write(terminal, prompt_message, strlen(prompt_message));
}

int read_input(char *input) {
    ssize_t bytesRead = read(STDIN_FILENO, input, INPUT_BUFFER_SIZE - 1);
    if (bytesRead > 0) {
        input[bytesRead - 1] = '\0';
    }
    return bytesRead;
}

void get_command_status(int status, long elapsed_time_ms, char *status_string, size_t size) {
    if (WIFEXITED(status)) {
        snprintf(status_string, size, "[exit:%d|%ldms]", WEXITSTATUS(status), elapsed_time_ms);
    } else if (WIFSIGNALED(status)) {
        snprintf(status_string, size, "[sign:%d|%ldms]", WTERMSIG(status), elapsed_time_ms);
    } else {
        status_string[0] = '\0'; // Clear the status string
    }
}

int execute_command(char *command, long *elapsed_time_ms) {
    command[strcspn(command, "\n")] = '\0';
    struct timespec start, end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime (start)");
        exit(EXIT_FAILURE);
    }


    char *args[100]; // Assumption: maximum 100 arguments
    char *input_file = NULL, *output_file = NULL;
    char *token = strtok(command, " ");
    int i = 0;

    /*while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;*/
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            input_file = strtok(NULL, " ");
        } else if (strcmp(token, ">") == 0) {
            output_file = strtok(NULL, " ");
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execlp(command, command, (char *)NULL);
        execvp(args[0], args);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        int status;

        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
            perror("clock_gettime (end)");
            exit(EXIT_FAILURE);
        }

        // Calculate elapsed time in milliseconds
        *elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
        return status;
    }
}
int read_user_input(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        if (feof(stdin)) { // Handle Ctrl+D
            return 0;
        } else {
            perror("error while reading user input");
            exit(EXIT_FAILURE);
        }
    }
    return 1;
}

