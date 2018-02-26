#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int ksh_cat(char **args)
{
    int txt =-1 ;
    char st ;
    int len  = getArgslen(args)  ;
    if(len==2){
        txt = open(args[1],O_RDONLY|O_CREAT);
        if(txt != -1)
        {
            while(read(txt,&st,1))
                printf("%c",st);
            printf("\n");
            close(txt);
        }
        else {
            printf("文件不存在!\n") ;
        }
    }
    return 1 ;
}