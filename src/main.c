#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "man.c"
#include "cd.c"
#include "pwd.c"
#include "ls.c"
#include "cp.c"
#include "mv.c"
#include "rm.c"
#include "time.c"
#include "who.c"
#include "date.c"
#include "touch.c"
#include "grep.c"
#include "last.c"
#include "sort.c"
#include "mkdir.c"
#include "cat.c"
#include "whoami.c"
#include "head.c"
#include "kill.c"
#include "ps.c"
#include "tree.c"
#include "df.c"
/*
  Function Declarations for builtin shell commands:
 */
int ksh_cd(char **args);
int ksh_help(char **args);
int ksh_exit(char **args);
int ksh_mkdir(char **args);
int ksh_cat(char **args);
int ksh_whoami(char **args);
int ksh_head(char **args);
int ksh_kill(char **args);
int ksh_ps(char **args);
int ksh_tree(char **args);
int ksh_df(char **args);
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
  "who",
  "date",
  "touch",
  "grep",
  "last",
  "sort",
  "mkdir",
  "cat",
  "whoami",
  "head",
  "kill",
  "ps",
  "tree",
  "df",
  "man"
};

char currentDir[100];

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
  &ksh_who,
  &ksh_date,
  &ksh_touch,
  &ksh_grep,
  &ksh_last,
  &ksh_sort,
  &ksh_mkdir,
  &ksh_cat,
  &ksh_whoami,
  &ksh_head,
  &ksh_kill,
  &ksh_ps,
  &ksh_tree,
  &ksh_df,
  &ksh_man
};

int ksh_num_builtins() {
  /* 算出实现的指令数 */
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
    // 输入空指令
    return 1;
  }
  //如果执行的指令已经被实现,则执行已经实现的 , 如 cd 指令会执行 ksh_cd
  for (i = 0; i < ksh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  //若该指令未实现,则会执行linux自带的该指令
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
        // 遇到回车就返回该行指令
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
// " \t\r\n\a" 按照这些分隔符来分割字符串
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
    // 获取当前所在路径
      if(getcwd(currentDir, sizeof(currentDir)) == NULL)
          perror("ksh");
    printf("%s> ",currentDir); // ex. /home/lx>
    line = ksh_read_line();  // 读取一行指令
    args = ksh_split_line(line); // 分割开
    status = ksh_execute(args);  // 想指令传过去

    free(line);
    free(args);
  } while (status);
}

void ksh_init(void)
{
    // 获取环境变量 getenv
    char *homePath = getenv("HOME");
    // 跳转到家目录
    if (chdir(homePath) != 0) {
        perror("ksh");
    }
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

  // 转到当前用户目录
  ksh_init();

  // Run command loop.
  ksh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

