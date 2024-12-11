#include <stdio.h>
#include <string.h>
#include "enseash.h"

int main() {
    char input[INPUT_BUFFER_SIZE]; // Buffer to store user input
    ssize_t bytesRead; // Number of bytes read from user input
    char status_string[STATUS_SIZE] = ""; // String to hold the status of the last command

    // Display the initial welcome message to the user
    display_welcome_message();

    // Display the initial prompt (no status initially)
    display_prompt(status_string);

    while (1) { // Infinite loop to keep the shell running
        // Read user input into the input buffer
        bytesRead = read_input(input);

        // Check for EOF (Ctrl+D) or "exit" command to terminate the shell
        if (bytesRead == 0 || (bytesRead > 0 && strcmp(input, "exit") == 0)) {
            write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE)); // Display exit message
            break; // Exit the loop and terminate the shell
        }

        long elapsed_time_ms = 0; // Variable to store the execution time of the command
        int raw_status = execute_command(input, &elapsed_time_ms); // Execute the command and measure its status

        // Update the status string with the command's exit status and execution time
        get_command_status(raw_status, elapsed_time_ms, status_string, STATUS_SIZE);

        // Display the prompt with the updated status string
        display_prompt(status_string);
    }

    return 0; // Exit the program
}
