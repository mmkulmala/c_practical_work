#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* mostly taken from: https://github.com/brenns10/lsh/blob/master/src/main.c */
/*
  Function Declarations for builtin shell commands:
 */
int wish_cd(char **args);
int wish_path(char **args);
int wish_exit(char **args);
int print_error(char *str);

/*
  Error messages
*/
char error_message[30] = "An error has occurred\n";
char cd_error_message[30] = "Expected argument to \"cd\"\n";
char exit_error_message[30] = "No argument to \"exit\"\n";
char forking_error_message[30] = "Forking error in \"launch\"\n";
char cp_error_message[30] = "Child process in \"launch\"\n";
char unable_to_read_line[40] = "Unable to getline in \"wish_read_line\"\n";
char allocation_error[20] = "Allocation error\n";

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "path",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &wish_cd,
  &wish_path,
  &wish_exit
};

int wish_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int wish_cd(char **args)
{
  if (args[1] == NULL) {
    print_error(cd_error_message);
  } else {
    if (chdir(args[1]) != 0) {
      print_error(error_message);
    }
  }
  return 1;
}

/**
   @brief Builtin command: add to path. Starts with /bin
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int wish_path(char **args)
{
  char *local_args[] = {"/bin", (char *) NULL};
  execvp("/bin", local_args);
  if (args[1] != NULL) {
    
  } else {
    fprintf(stdout, "path is: %s\n", local_args[0]);
  }
  
  return 1;
}

/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int wish_exit(char **args)
{
  if (args[1] != NULL) {
    print_error(exit_error_message);
    return 1;
  }
  return 0;
}

/**
 * @brief Print error messages
 * 
 */
int print_error(char *str) {
  write(STDERR_FILENO, str, strlen(str)); 
  return 1;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int wish_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      print_error(cp_error_message);
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    print_error(forking_error_message);
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int wish_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < wish_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return wish_launch(args);
}

/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *wish_read_line(void)
{
#ifdef LSH_USE_STD_GETLINE
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  if (getline(&line, &bufsize, stdin) == -1) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);  // We received an EOF
    } else  {
      print_error(unable_to_read_line);
      exit(EXIT_FAILURE);
    }
  }
  return line;
#else
#define LSH_RL_BUFSIZE 1024
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    print_error(allocation_error);
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        print_error(allocation_error);
        exit(EXIT_FAILURE);
      }
    }
  }
#endif
}

#define WISH_TOK_BUFSIZE 64
#define WISH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **wish_split_line(char *line)
{
  int bufsize = WISH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    print_error(allocation_error);
    exit(EXIT_FAILURE);
  }

  token = strtok(line, WISH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += WISH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        print_error(allocation_error);
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, WISH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void wish_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("wish> ");
    line = wish_read_line();
    args = wish_split_line(line);
    status = wish_execute(args);

    free(line);
    free(args);
  } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  wish_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}