#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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


// read_line_v2 uses c getline
char *ghell_read_line_v2(void)
{
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us

  if (getline(&line, &bufsize, stdin) == -1){
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);  // We recieved an EOF
    } else  {
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }

  return line;
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
