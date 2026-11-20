#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "parser.h"
#include "cmd.h"

int ghell_launch(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // child process
        if (execvp(args[0], args) == -1) {
            perror("ghell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // error forking
        perror("ghell");
    } else {
        // parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

// checks if a command matches a builtin and call it, or fails if not
int ghell_execute(char **args) {
    int i;

    if (args[0] == NULL) {
        // empty command entered
        return 1;
    }

    for (i = 0; i < ghell_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return ghell_launch(args);
}


void ghell_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = ghell_read_line();
        args = ghell_split_line(line);
        status = ghell_execute(args);

        free(line);
        free(args);
    } while (status);
}

int main() {
    ghell_loop();

    return EXIT_SUCCESS;
}