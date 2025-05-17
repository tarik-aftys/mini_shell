// executor.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // Pour strcmp, strlen
#include <unistd.h>     // Pour fork, execvp, chdir, dup2
#include <sys/types.h>
#include <sys/wait.h>   // Pour waitpid
#include <fcntl.h>      // Pour open
#include "shell.h"      // Prototypes et constantes
#include <pwd.h>   // pour getpwuid
#include <limits.h> // pour PATH_MAX
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

// Affiche le prompt du shell
char *get_prompt() {
    static char prompt[PATH_MAX + 100];
    char cwd[PATH_MAX];
    char *user = getpwuid(getuid())->pw_name;

    getcwd(cwd, sizeof(cwd));
    snprintf(prompt, sizeof(prompt),
             "\033[1;32m[%s@\033[1;34mmini-shell\033[1;32m %s]\033[0m$> ",
             user, cwd);
    return prompt;
}


// Lit une ligne depuis l'entrée standard

char *read_line() {
    char *line = readline("\0"); // Prompt nul car on gère le prompt ailleurs
    if (line && *line) {
        add_history(line); // Ajoute à l'historique si non vide
    }
    return line;
}

// Détecte s’il y a un pipe et exécute une pipeline
int contains_pipe(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0)
            return 1;
    }
    return 0;
}

void execute_pipeline(char **args) {
    char *commands[16][MAX_ARGS];  // max 16 sous-commandes, adapt si besoin
    int cmd_count = 0;
    int arg_index = 0;
    int pipe_index = 0;

    // Split args en sous-commandes autour de '|'
    while (args[arg_index] != NULL) {
        if (strcmp(args[arg_index], "|") == 0) {
            commands[cmd_count][pipe_index] = NULL;
            cmd_count++;
            pipe_index = 0;
        } else {
            commands[cmd_count][pipe_index++] = args[arg_index];
        }
        arg_index++;
    }
    commands[cmd_count][pipe_index] = NULL;
    cmd_count++;

    int pipefd[2], prev_fd = -1;

    for (int i = 0; i < cmd_count; i++) {
        if (i < cmd_count - 1 && pipe(pipefd) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Redirige stdin depuis le pipe précédent
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            // Redirige stdout vers le pipe actuel
            if (i < cmd_count - 1) {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            execvp(commands[i][0], commands[i]);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }

        // Parent : ferme les pipes inutiles
        if (prev_fd != -1) close(prev_fd);
        if (i < cmd_count - 1) {
            close(pipefd[1]);
            prev_fd = pipefd[0];
        }
    }

    // Attente de tous les enfants
    for (int i = 0; i < cmd_count; i++) {
        wait(NULL);
    }
}


// Exécute une commande avec ou sans redirection
void execute_command(char **args, int background) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Processus enfant

        char *input_file = NULL;
        char *output_file = NULL;

        // Vérifie si redirection présente dans les arguments
        check_redirection(args, &input_file, &output_file);

        // Redirection d'entrée si nécessaire
        if (input_file) {
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in < 0) {
                perror("Input redirection failed");
                exit(EXIT_FAILURE);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        // Redirection de sortie si nécessaire
        if (output_file) {
            int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0) {
                perror("Output redirection failed");
                exit(EXIT_FAILURE);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        // Exécute la commande
        execvp(args[0], args);
        perror("exec failed");
        exit(EXIT_FAILURE);
    } else {
        // Processus parent
        if (!background) {
            waitpid(pid, NULL, 0);  // Attente du processus fils
        } else {
            printf("[background pid: %d]\n", pid);
        }
    }
}

// Analyse les redirections dans les arguments et met à jour les fichiers
int check_redirection(char **args, char **input_file, char **output_file) {
    int i = 0;
    *input_file = NULL;
    *output_file = NULL;

    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0 && args[i + 1]) {
            *input_file = args[i + 1];
            args[i] = NULL;  // coupe la chaîne ici
            i++;
        } else if (strcmp(args[i], ">") == 0 && args[i + 1]) {
            *output_file = args[i + 1];
            args[i] = NULL;  // coupe la chaîne ici
            i++;
        }
        i++;
    }

    return 0;
}
