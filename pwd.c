#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// TODO: the flags and subcommands should be concentrated into a single
// unexported definition to help building the help and selecting the correct
// command/flag in controller.
int pwd_physical() {
    char *buffer = malloc(sizeof(char) * 512);
    // TODO: understand clearly the difference between --physical and --logical
    char *cwd = getcwd(buffer, sizeof(char) * 512 + 1);
    if (cwd == NULL) {
        perror("ghell");
        return 0;
    }

    printf("%s\n", cwd);
    free(buffer);
    return 1;
}
// TODO: create helper to help build those helps!
void pwd_help() {
    printf("\npwd - print name of current/working directory\n");
    printf("Print the full filename of the current working directory.\n\n");
    printf("  -L, --logical\tuse PWD from environment, even if it contains symlinks\n");
    printf("  -P, --physical\tresolve all symlinks\n");
    printf("  --help  \tdisplay this help and exit\n");
    printf("  --version\toutput version information and exit\n");
    printf("\t\nIf no option is specified, -P is assumed.\n\n");

    return;
}

int ghell_pwd_controller(char **args) {
    if (sizeof(args) > 1) {
        // TODO: move this check to a func
        if (args[1] == NULL || (strcmp(args[1], "-P") == 0 || strcmp(args[1], "--physical") == 0)) {
            return pwd_physical();
        } else {
            perror("ghell: invalid args for pwd command");
            pwd_help();
        }

        return 1;
    }

    return 1;
}
