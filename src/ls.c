#include <dirent.h>
#include <string.h>
#include "utils.c"

// TODO: 增加对-l参数支持

int ksh_ls(char **args)
{
    DIR *dirptr = NULL;
    struct dirent *entry;

    int len = getArgslen(args);

    if(len == 1)
        dirptr = opendir(".");
    else if(len == 2)
        dirptr = opendir(args[1]);
    else
        printf("Input error!\n");
    if(dirptr == NULL)
        perror("ksh");
    while ((entry = readdir(dirptr)) != 0)
    {
        if(strcmp(entry->d_name, ".") == 0)
            continue;
        if(strcmp(entry->d_name, "..") == 0)
            continue;
        printf("%s ", entry->d_name);
    }
    printf("\n");
    closedir(dirptr);
    return 1;
}