#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int ksh_cat(char **args)
{
    /**
     * @author lx
     * cat file
     * cat -n file  打印出行号
     */
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
    }else if (len==3 ){
        if(args[1][0] == '-'){
            if ( args[1][1] == 'n'){
                int count_ =0 ;
                txt = open(args[2],O_RDONLY|O_CREAT);
                if(txt != -1)
                {
                    printf("%d  ",++count_) ;
                    while(read(txt,&st,1))
                    {
                        printf("%c",st);
                        if(st=='\n'){
                            printf("%d  ",++count_) ;
                        }
                    }
                    printf("\n");
                    close(txt);
                }
                else {
                    printf("文件不存在!\n") ;
                }

            }
        }else {
            perror("ksh") ;
        }
    }
    else {
        perror("ksh") ;
    }

    return 1 ;
}