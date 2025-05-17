// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"
#include <readline/readline.h>
#include <readline/history.h>


int main() {
    char *line;
    char *args[MAX_ARGS];
    int background;

    while (1) {
        line = readline(get_prompt());
        if (line && *line) {
            add_history(line);
        }

        if (line == NULL) {
            printf("\n");
            break;
        }

        if (strlen(line) == 0) {
            free(line);
            continue;
        }

        parse_line(line, args, &background);

        // Commande interne : exit
        if (args[0] && strcmp(args[0], "exit") == 0) {
            free(line);
            break;
        }

        // Commande interne : cd
        if (args[0] && strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "cd: missing argument\n");
            } else if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
            free(line);
            continue;
        }

        // Commande interne : pwd
        if (args[0] && strcmp(args[0], "pwd") == 0) {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("pwd failed");
            }
            free(line);
            continue;
        }

        // Commande interne : clear
        if (args[0] && strcmp(args[0], "clear") == 0) {
            printf("\033[H\033[J"); // Séquence ANSI pour effacer l'écran
            free(line);
            continue;
        }

        // Si commande contient un pipe
        if (contains_pipe(args)) {
            execute_pipeline(args);
        } else {
            execute_command(args, background);
        }

        free(line);
    }

    return 0;
}
