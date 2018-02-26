#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int ksh_kill(char **args)
{
    int res = kill(atoi( args[1]) ,SIGKILL ) ;
    if(res==-1)
        perror("ksh") ;
    return 1 ;
}