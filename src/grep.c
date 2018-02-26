#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>
#include <fcntl.h>

int ksh_grep(char **args)
{
    int fd,r,j=0;
    char temp,line[100];
    if((fd=open(args[2],O_RDONLY)) != -1)
    {
        while((r=read(fd,&temp,sizeof(char)))!= 0)
        {
            if(temp!='\n')
            {
                line[j]=temp;
                j++;
            }
            else
            {
                if(strstr(line,args[1])!=NULL)
                    printf("%s\n",line);
                memset(line,0,sizeof(line));
                j=0;
            }

        }
    }

    return 1;
}