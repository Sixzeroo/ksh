#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define COPY_MODE 0644

int ksh_touch(char **args)
{
    int out_fd;

    // 测试文件是否存在
    if(access(args[1], F_OK) == 0)
    {
        printf("File exist！\n");
    }
    else
    {
        if((out_fd = creat(args[1], COPY_MODE)) == -1)
            perror("ksh");
    }

    close(out_fd);

    return 1;
}