#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096
#define COPY_MODE 0644

// TODO: 判断是否为文件夹进行复制 -R参数

int ksh_cp(char **args)
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFER_SIZE];
    bool fgFileExist = false;
    char input;

    if((in_fd = open(args[1], O_RDONLY)) == -1)
        perror("ksh");

    // 测试目标文件是否存在
    if(access(args[2], F_OK) == 0)
        fgFileExist = true;
    else
    {
        fgFileExist = false;
        if((out_fd = creat(args[2], COPY_MODE)) == -1)
            perror("ksh");
    }

    while ((n_chars = read(in_fd, buf, BUFFER_SIZE)) > 0)
    {
        if(fgFileExist)
        {
            if((out_fd = open(args[2], O_WRONLY)) == -1)
                perror("ksh");
            // 写入
            if(write(out_fd, buf, n_chars) != n_chars)
                perror("ksh");
        } else
        {
            if((out_fd = creat(args[2], COPY_MODE)) == -1)
                perror("ksh");
            // 写入
            if(write(out_fd, buf, n_chars) != n_chars)
                perror("ksh");
        }
    }

    close(in_fd);
    close(out_fd);
    return 1;
}