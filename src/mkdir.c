#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "utils.c"

void create_dir(char *args ,char *permission )
{
    DIR * dir = opendir(args) ;
    if(dir==NULL)
    {
        int sta ;
        if(permission!=NULL)
            sta  = mkdir(args , permission ) ;
        else
            sta = mkdir(args,0775) ;
        if(sta==0)
            printf("目录创建成功 !\n") ;
        else
            printf("目录创建失败,请检查路径名称 !\n");
    }
    else
    {
        printf("该目录已存在!\n") ;
        closedir(dir) ;
    }

}

int ksh_mkdir(char **args)
{
    /**
     * TODO
     * mkdir path
     * mkdir -m 700 path
     * mkdir -p path
     * mkdir --help
     *
     */
    int len  = getArgslen(args)  ;
//    printf("%d\n",len) ;
    if( len==2 ){
        // 1. 参数仅仅是路径
        create_dir(args[1],NULL) ;
    }


    return 1;
}