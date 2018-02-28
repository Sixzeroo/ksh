#include <dirent.h>
#include <string.h>
#include <string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include "utils.c"

// 返回文件模式的权限信息
int ksh_getpermission(char *fileName, char *temp)
{
    struct stat file_stat;

    if(0 > stat(fileName, &file_stat))
        perror("ksh");

    mode_t mode = file_stat.st_mode;
    // 文件的类型和存取的权限数据结构  值为00400
    mode_t perm = S_IREAD;

    if(S_IRUSR & mode) temp[0]='r';else temp[0]='-';
    if(S_IWUSR & mode) temp[1]='w';else temp[1]='-';
    if(S_IXUSR & mode) temp[2]='x';else temp[2]='-';
    if(S_IRGRP & mode) temp[3]='r';else temp[3]='-';
    if(S_IWGRP & mode) temp[4]='w';else temp[4]='-';
    if(S_IXGRP & mode) temp[5]='x';else temp[5]='-';
    if(S_IROTH & mode) temp[6]='r';else temp[6]='-';
    if(S_IWOTH & mode) temp[7]='w';else temp[7]='-';
    if(S_IXOTH & mode) temp[8]='x';else temp[8]='-';

    return 1;
}


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

    if(l_symbol)
        printf("%-30s\t%-13s\t%-8s\t%-8s\t%-6s\n","名称", "权限", "拥有者ID", "拥有者群组ID", "类型");

    // 读取文件信息
    while ((entry = readdir(dirptr)) != 0)
    {
        if(!a_symbol && entry->d_name[0] == '.')
            continue;
        // 文件权限承载字符串
        char filemod[10] = {'-','-','-','-','-','-','-','-','-','\0'};

        struct stat file_stat;
        if(0 > stat(entry->d_name, &file_stat))
            perror("ksh");

        // 文件夹显示不同的信息
        if(entry->d_type == 4)
        {
            if(l_symbol)
            {
                int res = ksh_getpermission(entry->d_name, filemod);
                printf("\e[0;36m %-30s\t%-13s\t%-8d\t%-8d\t%-6s\n\e[0m",entry->d_name, filemod, file_stat.st_uid, file_stat.st_gid, "dir");

            }
            else
                printf("\e[0;36m %s\t\e[0m", entry->d_name);
        }
        else
        {
            if(l_symbol)
            {
                int res = ksh_getpermission(entry->d_name, filemod);
                printf("%-30s\t%-13s\t%-8d\t%-8d\t%-6s\n",entry->d_name, filemod, file_stat.st_uid, file_stat.st_gid, "file");

            }
            else
                printf("%s\t", entry->d_name);
        }

    }

    printf("\n");
    closedir(dirptr);
    return 1;
}