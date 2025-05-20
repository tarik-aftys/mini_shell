#include <string.h>
#include "shell.h"

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
