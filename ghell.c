#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
  Function Declarations for builtin shell commands:
 */
int ghell_cd(char **args);
int ghell_help(char **args);
int ghell_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &ghell_cd,
    &ghell_help,
    &ghell_exit
};

int ghell_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
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
    printf("Fotkurz GHELL\n");
    printf("Type the program name and the args, hit enter and enjoy.\n");
    printf("Implemented builtins:\n");

    for (i = 0; i < ghell_num_builtins(); i++) {
        printf("\t%s\n", builtin_str[i]);
    }

    printf("\nUse the man command for information on other programs.\n");
    return 1;
}

int ghell_exit(char **args) {
    return 0;
}

// initial buff size for user input
#define GHELL_RL_BUFSIZE 1024

// lsh_read_line reads input line allocating mem at the size of the input, 
// returns a char *.
// modern c stdio.h have a getline() func that already implements it, but I'll
// keep this one 
char *ghell_read_line(void) {
    int bufsize = GHELL_RL_BUFSIZE;
    int position = 0;
    int c;
    char *buffer = malloc(sizeof(char) * bufsize);

    if (!buffer) {
        fprintf(stderr, "ghell: input allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read a character as a int, this way we can compare with EOF
        c = getchar();
        
        // if hit EOF (or end of line) replace it with a null char and return
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        // reallocates with more 1024 if we hit the max buffer size
        if (position >= bufsize) {
            bufsize += GHELL_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "ghell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

// tokenizer buff size
#define GHELL_TOK_BUFSIZE 64
// delimiter for tokenizer
#define GHELL_TOK_DELIM " \t\r\n\a"

// splits a char array into multiple args using the GHELL_TOK_DELIM as delim.
// Dynamically expands the buf size if required.
// returns the splitted tokens.
char **ghell_split_line(char *line) {
    int bufsize = GHELL_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "ghell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    // tokenize the strings using the delim as delimiters
    // strtok return pointers to within the string and place
    // \0 bytes at the end of each token.
    token = strtok(line, GHELL_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += GHELL_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "ghell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, GHELL_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

// read_line_v2 uses c getline
// char *read_line_v2(void)
// {
//   char *line = NULL;
//   ssize_t bufsize = 0; // have getline allocate a buffer for us

//   if (getline(&line, &bufsize, stdin) == -1){
//     if (feof(stdin)) {
//       exit(EXIT_SUCCESS);  // We recieved an EOF
//     } else  {
//       perror("readline");
//       exit(EXIT_FAILURE);
//     }
//   }

//   return line;
// }

int ghell_launch(char **args) {
    pid_t pid, wpid;
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
            wpid = waitpid(pid, &status, WUNTRACED);
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
    // INIT

    // read
    ghell_loop();
    // parsed
    // execute

    // terminate
    return EXIT_SUCCESS;
}