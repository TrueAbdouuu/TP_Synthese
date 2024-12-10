#include <stdio.h>
#include <string.h>
#include "enseash.h"

int main() {
    char input[INPUT_BUFFER_SIZE];
    ssize_t bytesRead;
    char status_string[STATUS_SIZE] = "";

    display_welcome_message();

    display_prompt(status_string);

    while (1) {
        bytesRead = read_input(input);

        if (bytesRead == 0 || (bytesRead > 0 && strcmp(input, "exit") == 0)) {
            write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
            break;
        }
        long elapsed_time_ms = 0;
        int raw_status = execute_command(input, &elapsed_time_ms);
        get_command_status(raw_status, elapsed_time_ms, status_string, STATUS_SIZE);

        display_prompt(status_string);
    }

    return 0;
}
