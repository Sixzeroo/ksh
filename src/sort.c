#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>
#include <fcntl.h>

int ksh_sort(char **args)
{
    int fd,r,j=0;
    char lines[100][100],line[100],tem,sort_tem[100];
    int len=0;

    // 读取数据
    if((fd = open(args[1], O_RDONLY)) != -1)
    {
        while ((r = read(fd, &tem, sizeof(char))) != 0)
        {
            if(tem != '\n')
            {
                line[j++] = tem;
            }
            else
            {
                strcpy(lines[len++], line);
                memset(line, 0, sizeof(line));
                j=0;
            }
        }
    }

    close(fd);

    // 排序
    for(int i=0;i<len;++i)
    {
        for(int j=0;j<len-i-1;++j)
        {
            if(strcmp(lines[j], lines[j+1]) > 0)
            {
                strcpy(sort_tem, lines[j]);
                strcpy(lines[j], lines[j+1]);
                strcpy(lines[j+1], sort_tem);
            }
        }
    }

    for(int i=0;i<len;++i)
        printf("%s\n",lines[i]);

    return 1;
}