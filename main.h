#ifndef MAIN_H
#define MAIN_H

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

/* Function prototypes */
char **parse_command(char *input);
int execute_command(char **args);
int shell_cd(char **args);
int shell_exit(char **args);
int shell_help(char **args);
int num_builtins();

#endif /* MAIN_H */