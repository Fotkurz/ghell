#ifndef CMD_H
#define CMD_H

/*
Function Declarations for builtin shell commands:
*/
int ghell_cd(char **args);
int ghell_help(char **args);
int ghell_exit(char **args);

int ghell_num_builtins();

/*
  Identifiers Declarations
*/
extern char *builtin_str[];
extern int (*builtin_func[]) (char **);

#endif