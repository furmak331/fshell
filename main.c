#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

#define PROMPT "fShell$ "
#define PROMPT_LENGTH 8
#define BUFFER_SIZE 1024
#define MAX_ARGS 64


char **parse_command(char *input);
int execute_command(char **args);
int shell_cd(char **args);
int shell_exit(char **args);
int shell_help(char **args);


char *builtin_commands[] = {
    "cd",
    "exit",
    "help"
};

int (*builtin_functions[])(char **) = {
    &shell_cd,
    &shell_exit,
    &shell_help
};

int num_builtins() {
    return sizeof(builtin_commands) / sizeof(char *);
}

int shell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "fshell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("fshell");
        }
    }
    return 1;
}

int shell_exit(char **args) {
    (void)args; 
    return 0;
}

int shell_help(char **args) {
    (void)args; 
    
    printf("fShell - A simple custom UNIX shell\n");
    printf("Type program names and arguments, then hit enter.\n");
    printf("The following commands are built in:\n");
    
    for (int i = 0; i < num_builtins(); i++) {
        printf("  %s\n", builtin_commands[i]);
    }
    
    printf("Use the man command for information on other programs.\n");
    return 1;
}


char **parse_command(char *input) {
    int buffer_size = MAX_ARGS;
    int position = 0;
    char **tokens = malloc(buffer_size * sizeof(char *));
    char *token;
    
    if (!tokens) {
        fprintf(stderr, "fshell: allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    token = strtok(input, " \t\r\n\a");
    while (token != NULL) {
        tokens[position] = token;
        position++;
        
        if (position >= buffer_size) {
            buffer_size += MAX_ARGS;
            tokens = realloc(tokens, buffer_size * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "fshell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        
        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}


int execute_command(char **args) {
    int i;
    
    if (args[0] == NULL) {

        return 1;
    }
    

    for (i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_commands[i]) == 0) {
            return (*builtin_functions[i])(args);
        }
    }
    

    pid_t pid, wpid;
    int status;
    
    pid = fork();
    if (pid == 0) {

        if (execvp(args[0], args) == -1) {
            perror("fshell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {

        perror("fshell");
    } else {
     
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    char **args;
    int status;
    
    (void)argc, (void)argv; // Suppress unused parameter warnings
    
    status = 1;  
    while (status) {
        write(STDOUT_FILENO, PROMPT, PROMPT_LENGTH);
        
        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        
        if (bytes_read <= 0) {
            write(STDOUT_FILENO, "\nExiting fshell :) \n", 21);
            break;
        }
        
        if (bytes_read > 0 && buffer[bytes_read - 1] == '\n') {
            buffer[bytes_read - 1] = '\0';
        } else {
            buffer[bytes_read] = '\0';
        }
        
        args = parse_command(buffer);
        status = execute_command(args);
        
        free(args);
    }
    
    return EXIT_SUCCESS;
}