#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int ksh_head(char **args)
{
    /**
     * @author lx
     * 参数 -n
     * example : head 1.txt    默认显示文件的前5行
     *           head -n 10 1.txt 显示文件的前10行
     */
    int txt =-1 ;
    char st ;
    int len  = getArgslen(args)  ;
    int count=5 ;
    char * filename ="" ;
    if(len==2){
        filename =args[1] ;
    }else if(len==4){
        // head  -n  数字  file
        if(args[1][0]=='-' && args[1][1]=='n'){
            int line_num = atoi(args[2]) ;
            count = line_num ;
            filename = args[3] ;
        }
    }else {
        perror("参数错误\n") ;
    }

    txt = open(filename,O_RDONLY);
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

    return 1 ;
}