#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cd.c"
#include "pwd.c"
#include "ls.c"
#include "cp.c"
#include "mv.c"
#include "rm.c"
#include "time.c"
#include "who.c"

/*
  Function Declarations for builtin shell commands:
 */
int ksh_cd(char **args);
int ksh_help(char **args);
int ksh_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "pwd",
  "ls",
  "cp",
  "mv",
  "rm",
  "time",
  "who"
};

int (*builtin_func[]) (char **) = {
  &ksh_cd,
  &ksh_help,
  &ksh_exit,
  &ksh_pwd,
  &ksh_ls,
  &ksh_cp,
  &ksh_mv,
  &ksh_rm,
  &ksh_time,
  &ksh_who
};

int ksh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int ksh_help(char **args)
{
  int i;
  printf("***************************\n");
  printf("OS课设——KSH简单实现\n");
  printf("***************************\n");
  printf("输入命令名称和参数，按下回车执行\n");
  printf("支持以下命令:\n");

  for (i = 0; i < ksh_num_builtins(); i++) {
    printf("%d.  %s\n", i+1, builtin_str[i]);
  }

  return 1;
}

/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int ksh_exit(char **args)
{
  return 0;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int ksh_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("ksh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("ksh");
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
int ksh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < ksh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  printf("Use system shell:\n");
  return ksh_launch(args);
}

#define KSH_RL_BUFSIZE 1024
/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *ksh_read_line(void)
{
  int bufsize = KSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "ksh: allocation error\n");
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
      bufsize += KSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "ksh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define KSH_TOK_BUFSIZE 64
#define KSH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **ksh_split_line(char *line)
{
  int bufsize = KSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "ksh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, KSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += KSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        fprintf(stderr, "ksh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, KSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void ksh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("> ");
    line = ksh_read_line();
    args = ksh_split_line(line);
    status = ksh_execute(args);

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
  ksh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

