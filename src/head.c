#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int ksh_head(char **args)
{
    int txt =-1 ;
    char st ;
    int len  = getArgslen(args)  ;
    int count=5 ;
    if(len==2){
        txt = open(args[1],O_RDONLY);
        if(txt != -1)
        {
            while(read(txt,&st,1) )
            {
                if(st=='\n')
                {
                    count-- ;
                    if(count<=0)
                        break ;
                }
                printf("%c",st);
            }
            printf("\n");
            close(txt);
        }
        else {
            printf("文件不存在!\n") ;
        }
    }
    return 1 ;
}