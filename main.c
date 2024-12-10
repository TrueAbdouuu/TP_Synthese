#include <stdio.h>
#include <string.h>
#include "enseash.h"

int main() {
    char input[INPUT_BUFFER_SIZE];
    ssize_t bytesRead;


    display_welcome_message();
    display_prompt();

    while (1) {

        bytesRead = read_input(input);


        if (bytesRead == 0 || (bytesRead > 0 && strcmp(input, "exit") == 0)) {
            write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
            break;
        }


        execute_command(input);
        display_prompt();
    }

    return 0;
}
