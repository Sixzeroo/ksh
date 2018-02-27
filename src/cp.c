#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096
#define COPY_MODE 0644

bool ksh_isdir(char *filename)
{
    struct stat fileInfo;

    if (stat(filename, &fileInfo) >= 0)
        if (S_ISDIR(fileInfo.st_mode))
            return true;
        else
            return false;
}


int ksh_cpf2f(char *src_file, char *dest_file)
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFER_SIZE];
    bool fgFileExist = false;
    char input;

    if((in_fd = open(src_file, O_RDONLY)) == -1)
        perror("ksh");

    // 测试目标文件是否存在
    if(access(dest_file, F_OK) == 0)
        fgFileExist = true;
    else
    {
        fgFileExist = false;
        if((out_fd = creat(dest_file, COPY_MODE)) == -1)
            perror("ksh");
    }

    while ((n_chars = read(in_fd, buf, BUFFER_SIZE)) > 0)
    {
        if(fgFileExist)
        {
            if((out_fd = open(dest_file, O_WRONLY)) == -1)
                perror("ksh");
            // 写入
            if(write(out_fd, buf, n_chars) != n_chars)
                perror("ksh");
        } else
        {
            if((out_fd = creat(dest_file, COPY_MODE)) == -1)
                perror("ksh");
            // 写入
            if(write(out_fd, buf, n_chars) != n_chars)
                perror("ksh");
        }
    }

    close(in_fd);
    close(out_fd);
}

int ksh_cpdir(char *src_dir, char *dest_dir)
{
    DIR *dp = NULL;
    struct dirent *dirp;
    char tempDest[256];
    char tempSrc[256];
    strcpy(tempDest, dest_dir);
    strcpy(tempSrc, src_dir);

    /*使用opendir函数打开src_dir目录，获得指向该目录名字的指针*/
    if ((dp = opendir(src_dir)) == NULL)
        return 1;
    else
    {
        /*使用readdir函数读取dp所指代目录的目录项，获得指向下一个dirent结构的指针*/
        while ((dirp = readdir(dp)))
        {
            struct stat file_stat;
            if (!ksh_isdir(dirp->d_name))
            {
                /*将dirent结构中的d_name成员变量链接到上级目录字符串*/
                strcat(tempDest, dirp->d_name);
                strcat(tempSrc, dirp->d_name);

                /*此处转换为文件复制函数的方式处理目录复制*/
                ksh_cpf2f(tempSrc, tempDest);

                /*通过字符串拷贝函数，将tempDest和tempSrc还原为上级的目录名*/
                strcpy(tempDest, dest_dir);
                strcpy(tempSrc, src_dir);
            }
        }
        /*关闭目录*/
        closedir(dp);
        return 0;
    }
}

int ksh_cp(char **args)
{
    int r_symbol = 0;
    int len = getArgslen(args);

    if(len == 3)
        ksh_cpf2f(args[1], args[2]);
    else if(len == 4)
    {
        if(args[1][0] == '-' && args[1][1] == 'r')
            ksh_cpdir(args[1], args[2]);
        else
            printf("输入不符合规范!\n");
    }
    else
        printf("输入不符合规范!\n");

    return 1;
}