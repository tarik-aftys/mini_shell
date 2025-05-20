#ifndef SHELL_H
#define SHELL_H
#define MAX_INPUT 1024       
#define MAX_ARGS 64         
#ifndef PATH_MAX
#define PATH_MAX 4096      
#endif
void print_prompt();                   
char *read_line();                    
int parse_line(char *line, char **args, int *background);  
char *get_prompt();  
void execute_command(char **args, int background);    
int check_redirection(char **args, char **input_file, char **output_file); 
int contains_pipe(char **args);    
void execute_pipeline(char **args);   
#endif

