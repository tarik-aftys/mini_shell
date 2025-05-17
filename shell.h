// shell.h
#ifndef SHELL_H
#define SHELL_H

// ========== Constantes ==========
#define MAX_INPUT 1024       // Longueur max d'une ligne de commande
#define MAX_ARGS 64          // Nombre max d'arguments par commande

#ifndef PATH_MAX
#define PATH_MAX 4096        // Taille max du chemin courant (sécurité)
#endif

// ========== Fonctions générales ==========
void print_prompt();                   // Affiche le prompt coloré
char *read_line();                     // Lit une ligne de commande

// ========== Parsing ==========
int parse_line(char *line, char **args, int *background);  // Découpe la ligne en arguments
char *get_prompt();  // déclaration

// ========== Commande simple ==========
void execute_command(char **args, int background);         // Exécute une commande avec ou sans redirection
int check_redirection(char **args, char **input_file, char **output_file); // Gère > et <

// ========== Pipe ==========
int contains_pipe(char **args);       // Détecte la présence de '|'
void execute_pipeline(char **args);   // Exécute une ou plusieurs commandes séparées par des pipes

#endif // SHELL_H

