#include <stdio.h>
#include <string.h>
#include "enseash.h"

int main() {
    char input[INPUT_BUFFER_SIZE];  // Buffer to store user input
    ssize_t bytesRead;  // Variable to store the number of bytes read from user input

    // Display the welcome message at the start
    display_welcome_message();

    // Display the prompt for user input
    display_prompt();

    while (1) {
        // Read the user input and store it in 'input'
        bytesRead = read_input(input);

        // Check if user wants to exit (either by typing "exit" or pressing Ctrl+D)
        if (bytesRead == 0 || (bytesRead > 0 && strcmp(input, "exit") == 0)) {
            write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));  // Display "Bye bye..." message
            break;  // Exit the loop and terminate the program
        }

        // Execute the command entered by the user
        execute_command(input);

        // Display the prompt again after command execution
        display_prompt();
    }

    return 0;
}
