# ENSEA Tiny Shell (enseash)

## **Important Note**

**Since the start of this year, I have been doing all TPs alone and completing all compte rendus alone. This workload is taking a toll on me as I cannot progress at the same speed as others working in pairs (binômes). I am effectively doing twice the work because I have to prepare two compte rendus every week while also focusing on exams and other responsibilities. As a result, my TPs may lag behind others in terms of progress. I sincerely hope this will be taken into account during the final evaluation of this TP. Thank you.**

---

## Objective

The primary goal of this practical work is to create a micro shell, `enseash`, capable of launching commands, measuring execution times, and managing input/output redirections.

---

## Step-by-Step Implementation

### **Step 1: Welcome Message and Initial Prompt**
The shell begins by displaying a welcome message and a prompt. For example:
```
Welcome to ENSEA Tiny Shell.
Type 'exit' to quit.
enseash %
```
The `write()` function is used to send the welcome message to the terminal. The file descriptor `STDOUT_FILENO` is passed to `write()`, along with the string and its length.
```c
int terminal = STDOUT_FILENO;
write(terminal, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
```
### **Step 2: Command Execution and Return to Prompt**
The shell operates in a Read-Eval-Print Loop (REPL):
1. Read the user’s input using the `read()` function.
2. Execute the input command using `fork()` and `execvp()`.
3. Return to the prompt to wait for the next command.

```c
ssize_t bytesRead = read(STDIN_FILENO, input, INPUT_BUFFER_SIZE);
if (bytesRead > 0) {
    input[bytesRead - 1] = '\0'; // Remove newline
    execute_command(input, &elapsed_time_ms);
}
```

### **Step 3: Shell Exit with "exit" or Ctrl+D**
The shell terminates when the user types "exit" or presses Ctrl+D. The EOF condition is detected by checking the result of the `read()` function.
```c
if (strcmp(input, "exit") == 0 || bytesRead == 0) {
    write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
    break;
}
```

### **Step 4: Displaying Exit Codes and Signals in the Prompt**
The shell displays the exit code or signal of the previous command in the prompt. The `WIFEXITED()` and `WIFSIGNALED()` macros are used to determine the exit condition.
```c
if (WIFEXITED(status)) {
    snprintf(status_string, STATUS_SIZE, "[exit:%d]", WEXITSTATUS(status));
} else if (WIFSIGNALED(status)) {
    snprintf(status_string, STATUS_SIZE, "[sign:%d]", WTERMSIG(status));
}
```
### **Step 5: Measuring Execution Time**
Using the `clock_gettime()` function, the execution time of commands is measured in milliseconds and displayed in the prompt:
```c
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC, &start);
execute_command(input, &status);
clock_gettime(CLOCK_MONOTONIC, &end);
*elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
```
### **Step 6: Execution of Complex Commands with Arguments**
Commands with multiple arguments are parsed using the `strtok()` function. The arguments are stored in an array, which is passed to `execvp()` for execution.
```c
char *args[100];
int i = 0;
char *token = strtok(input, " ");
while (token != NULL) {
    args[i++] = token;
    token = strtok(NULL, " ");
}
args[i] = NULL;
execvp(args[0], args);
```
### **Step 7: Redirection of Input and Output**
Redirection is managed by identifying the `>` and `<` symbols in the command string. The input/output files are opened using `open()`, and their file descriptors are duplicated using `dup2()`.
```c
if (strcmp(token, ">") == 0) {
    output_file = strtok(NULL, " ");
    int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(fd_out, STDOUT_FILENO);
    close(fd_out);
}
```
---

## Features Completed
1. Displaying a welcome message and prompt.
2. Reading and executing simple commands.
3. Managing shell exit with "exit" and Ctrl+D.
4. Displaying exit codes and signals in the prompt.
5. Measuring and displaying command execution times.
6. Handling complex commands with arguments.
7. Managing redirection of stdin and stdout.

---

## Remaining Steps
- **Pipes (`|`)**: Implement command chaining with `pipe()`.
- **Background Execution (`&`)**: Allow non-blocking execution of commands and manage background processes.

---


