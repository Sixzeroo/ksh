#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int ksh_rmdir(const char *dir)
{
    char cur_dir[] = ".";
    char up_dir[] = "..";
    char dir_name[128];
    DIR *dirp;
    struct dirent *dp;
    struct stat dir_stat;

    // 参数传递进来的目录不存在，直接返回
    if ( 0 != access(dir, F_OK) ) {
        return 0;
    }

    // 获取目录属性失败，返回错误
    if ( 0 > stat(dir, &dir_stat) ) {
        perror("get directory stat error");
        return -1;
    }

    if ( S_ISREG(dir_stat.st_mode) ) {  // 普通文件直接删除
        remove(dir);
    } else if ( S_ISDIR(dir_stat.st_mode) ) {   // 目录文件，递归删除目录中内容
        dirp = opendir(dir);
        while ( (dp=readdir(dirp)) != NULL ) {
            // 忽略 . 和 ..
            if ( (0 == strcmp(cur_dir, dp->d_name)) || (0 == strcmp(up_dir, dp->d_name)) ) {
                continue;
            }

            sprintf(dir_name, "%s/%s", dir, dp->d_name);
            ksh_rmdir(dir_name);   // 递归调用
        }
        closedir(dirp);

        rmdir(dir);     // 删除空目录
    } else {
        perror("unknow file type!");
    }

    return 0;
}

int ksh_rm(char **args)
{
    int r_symbol = 0;
    char *name;

    int len = getArgslen(args);

    for(int i=1;i<len;i++)
    {
        if(args[i][0] == '-')
        {
            for(int j=1;args[i][j]!='\0';j++)
            {
                if(args[i][j] == 'r')
                    r_symbol = 1;
                else
                {
                    printf("Input Option is invalid!\n");
                    return 0;
                }
            }
        }
        else
        {
            name = args[i];
        }
    }


    struct stat buf;
    int result = stat(name, &buf);
    if(r_symbol)
    {
        // 文件夹情况
        if(S_ISDIR(buf.st_mode)){
            ksh_rmdir(name);
        }else if(S_ISREG(buf.st_mode)){
            // 文件情况
            if(remove(args[1]) == -1)
                perror("ksh");
        }
    }
    else
    {
        if(S_ISDIR(buf.st_mode)){
            printf("It is a directory\n");
        }else if(S_ISREG(buf.st_mode)){
            // 文件情况
            if(remove(args[1]) == -1)
                perror("ksh");
        }
    }

    return 1;
}