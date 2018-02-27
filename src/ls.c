#include <dirent.h>
#include <string.h>
#include "utils.c"


int ksh_ls(char **args)
{
    DIR *dirptr = NULL;
    struct dirent *entry;
    int l_symbol = 0, a_symbol = 0;

    int len = getArgslen(args);

    dirptr = opendir(".");

    // 解析参数
    for(int i=1;i<len;i++)
    {
        if(args[i][0] == '-')
        {
            for(int j=1;args[i][j]!='\0';j++)
            {
                if(args[i][j] == 'l')
                    l_symbol = 1;
                else if (args[i][j] == 'a')
                    a_symbol = 1;
                else
                {
                    printf("Input Option is invalid!\n");
                    return 0;
                }
            }
        }
        else
        {
            dirptr = opendir(args[i]);
        }
    }

    if(dirptr == NULL)
        perror("ksh");

    // 读取文件信息
    while ((entry = readdir(dirptr)) != 0)
    {
        if(!a_symbol)
        {
            if(entry->d_name[0] == '.')
                continue;
        }
        if(l_symbol)
        {
            printf("%s\t%s\n", entry->d_name, entry->d_type==4?"dir":"file");
        }
        else
            printf("%s\t", entry->d_name);
    }

    printf("\n");
    closedir(dirptr);
    return 1;
}