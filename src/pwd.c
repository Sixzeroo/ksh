#include <unistd.h>

#define BUFF_SIZE 80

int ksh_pwd(char **args)
{
    char buf1[BUFF_SIZE];
    if(getcwd(buf1, sizeof(buf1)) == NULL)
        perror("ksh");
    printf("%s\n", buf1);
    return 1;
}