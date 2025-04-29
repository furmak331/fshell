#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define PROMPT "fShell$ "
#define PROMPT_LENGTH 8
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    
    (void)argc, (void)argv;

    while (1) {

        write(STDOUT_FILENO, PROMPT, PROMPT_LENGTH);

        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);

        if (bytes_read <= 0) {
            write(STDOUT_FILENO, "\nExiting fshell :) \n", 21);
            break;
        }
        

        if (bytes_read > 0 && buffer[bytes_read - 1] == '\n') {
            buffer[bytes_read - 1] = '\0';
        }
        
        printf("Echoing input back: %s\n", buffer);
    }
    
    return (0);
}