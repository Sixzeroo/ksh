#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
int ksh_kill(char **args)
{
    /**
     * @author lx
     * 参数 :
     *      kill -数字 PID
     *      默认 kill -9 PID  彻底杀死进程
     * example : kill -9 1234
     */
    int step =9 ;
    int _pid = 0 ;
    if(getArgslen(args) ==3  )
    {
        if(args[1][0] == '-')
        {
            char* result = strchr(args[1], args[1][1] ) ;
            step = atoi(result) ;
            _pid = atoi(args[2]) ;
        }
        else
        {
            perror("参数错误") ;
        }

    }
    else if( getArgslen(args)!=2 )
    {
        perror("参数错误") ;
        return 1 ;
    }else {
        _pid = atoi(args[1]) ;
    }
    int res = kill(_pid ,step ) ;
    if(res==-1)
        perror("ksh") ;
    return 1 ;
}