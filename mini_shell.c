#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

// Fonction pour afficher le prompt
void print_prompt() {
    printf("$> ");
    fflush(stdout);
}

// Fonction pour lire une ligne de commande
char *read_line() {
    char *line = malloc(MAX_INPUT);
    if (!line) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    if (fgets(line, MAX_INPUT, stdin) == NULL) {
        free(line);
        return NULL;
    }
    // Supprimer le saut de ligne
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
    return line;
}

// Fonction pour parser la ligne en arguments
int parse_line(char *line, char **args, int *background) {
    int argc = 0;
    *background = 0;
    char *token = strtok(line, " ");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            *background = 1;
        } else {
            args[argc++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[argc] = NULL;
    return argc;
}

// Fonction pour exécuter une commande
void execute_command(char **args, int background) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        // Processus enfant
        if (execvp(args[0], args) == -1) {
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // Processus parent
        if (!background) {
            waitpid(pid, NULL, 0);
        } else {
            printf("[background pid: %d]\n", pid);
        }
    }
}

// Fonction principale
int main() {
    char *line;
    char *args[MAX_ARGS];
    int background;

    while (1) {
        print_prompt();
        line = read_line();
        if (line == NULL) {
            printf("\n");
            break;
        }

        // Ne rien faire si ligne vide
        if (strlen(line) == 0) {
            free(line);
            continue;
        }

        // Quitter le shell
        if (strcmp(line, "exit") == 0) {
            free(line);
            break;
        }

        // Commande "cd"
        if (strncmp(line, "cd ", 3) == 0) {
            char *path = line + 3;
            if (chdir(path) != 0) {
                perror("cd failed");
            }
            free(line);
            continue;
        }

        parse_line(line, args, &background);
        execute_command(args, background);
        free(line);
    }

    return 0;
}
