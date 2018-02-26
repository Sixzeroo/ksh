#include <stdio.h>

// TODO: 支持参数

int ksh_rm(char **args)
{
    if(remove(args[1]) == -1)
        perror("ksh");

    return 1;
}