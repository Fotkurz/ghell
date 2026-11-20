#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "cmd.h"
#include "pwd.h"


/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "pwd"
};

char *builtin_help_message[] = {
    "change directory",
    "display help message",
    "exit ghell program",
    "show current directory path"
};

int (*builtin_func[]) (char **) = {
    &ghell_cd,
    &ghell_help,
    &ghell_exit,
    &ghell_pwd
};

int ghell_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// /*
//   Builtin function implementations.
// */
int ghell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "ghell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("ghell");
        }
    }
    return 1;
}

int ghell_help(char **args) {
    int i;
    printf("Fotkurz GHELL\n\n");
    printf("Type the program name and the args, hit enter and enjoy.\n");
    printf("Implemented builtins:\n");

    for (i = 0; i < ghell_num_builtins(); i++) {
        printf("\t%s - %s\n", builtin_str[i], builtin_help_message[i]);
    }

    printf("\n  Use the man command for information on other programs. (e.g. man cd)\n");
    return 1;
}

int ghell_exit(char **args) {
    return 0;
}

int ghell_pwd(char **args) {
    return ghell_pwd_controller(args);
}