#include "enseash.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define EXIT_MESSAGE "Bye bye...\n"

// Displays the initial welcome message when the shell starts
void display_welcome_message() {
    int terminal = STDOUT_FILENO;
    write(terminal, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
}

// Displays the shell prompt with the current status string
void display_prompt(char *status_string) {
    int terminal = STDOUT_FILENO;
    char prompt_message[200];
    // Include the status in the prompt for better user feedback
    snprintf(prompt_message, sizeof(prompt_message), "enseash %s ", status_string);
    write(terminal, prompt_message, strlen(prompt_message));
}

// Reads user input from the terminal into the provided buffer
// Returns the number of bytes read or -1 on error
int read_input(char *input) {
    ssize_t bytesRead = read(STDIN_FILENO, input, INPUT_BUFFER_SIZE - 1);
    if (bytesRead > 0) {
        input[bytesRead - 1] = '\0'; // Replace the newline character with a null terminator
    }
    return bytesRead;
}

// Formats the status string to indicate how a command terminated (exit code or signal) and its execution time
void get_command_status(int status, long elapsed_time_ms, char *status_string, size_t size) {
    if (WIFEXITED(status)) {
        // Command exited normally, include the exit code and elapsed time
        snprintf(status_string, size, "[exit:%d|%ldms]", WEXITSTATUS(status), elapsed_time_ms);
    } else if (WIFSIGNALED(status)) {
        // Command terminated by a signal, include the signal number and elapsed time
        snprintf(status_string, size, "[sign:%d|%ldms]", WTERMSIG(status), elapsed_time_ms);
    } else {
        // Clear the status string if no valid information is available
        status_string[0] = '\0';
    }
}

// Executes the given command, measures its execution time, and returns its status
int execute_command(char *command, long *elapsed_time_ms) {
    command[strcspn(command, "\n")] = '\0'; // Remove trailing newline from command

    struct timespec start, end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime (start)");
        exit(EXIT_FAILURE);
    }

    char *args[100]; // Array to store command arguments (max 100)
    char *input_file = NULL, *output_file = NULL;
    char *token = strtok(command, " ");
    int i = 0;

    // Tokenize the command to handle arguments and I/O redirection
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            input_file = strtok(NULL, " "); // Capture input file
        } else if (strcmp(token, ">") == 0) {
            output_file = strtok(NULL, " "); // Capture output file
        } else {
            args[i++] = token; // Store command arguments
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // Null-terminate the arguments array

    pid_t pid = fork(); // Create a child process
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process: execute the command
        execlp(command, command, (char *)NULL); // Fallback to simple execution
        execvp(args[0], args); // Execute command with arguments
        perror("execlp");
        exit(EXIT_FAILURE); // Exit if exec fails
    } else {
        // Parent process: wait for the child to finish
        int status;

        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
            perror("clock_gettime (end)");
            exit(EXIT_FAILURE);
        }

        // Calculate the elapsed time in milliseconds
        *elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
        return status;
    }
}

// Reads user input using fgets, handling errors and EOF (e.g., Ctrl+D)
int read_user_input(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        if (feof(stdin)) {
            // Handle EOF (Ctrl+D) gracefully
            return 0;
        } else {
            perror("error while reading user input");
            exit(EXIT_FAILURE);
        }
    }
    return 1; // Input successfully read
}
