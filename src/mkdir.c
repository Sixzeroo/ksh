#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void create_dir(char *args ,char *permission )
{
    DIR * dir = opendir(args) ;
    if(dir==NULL)
    {

        int sta ;
        if(permission!=NULL)
            sta  = mkdir(args , atoi( permission)  ) ;
        else
            sta = mkdir(args,0775) ;
        if(sta==0)
            printf("目录创建成功 !\n") ;
        else
            perror("ksh") ;
    }
    else
    {
        printf("该目录已存在!\n") ;
        closedir(dir) ;
    }

}
#define BUFF_SIZE 80
int ksh_mkdir(char **args)
{
    /**
     * @author lx
     * mkdir path
     * mkdir -m 700 path  赋予相应的权限
     * mkdir -p path  递归创建文件夹
     */
    char * ptr ;
    int len  = getArgslen(args)  ;
//    printf("%d\n",len) ;
    if( len==2 ){
        // 1. 参数仅仅是路径
        create_dir(args[1],NULL) ;
    }else if(len ==3 && args[1][0]=='-' && args[1][1]=='p' ){
        ptr = strtok(args[2], "/");
        char buf1[BUFF_SIZE];
        getcwd(buf1, sizeof(buf1))  ;

        while (ptr != NULL) {
//            printf("%s\n",ptr) ;
            mkdir( ptr,0775) ;
            chdir(ptr) ;
            ptr = strtok(NULL, "/");
        }
        chdir(buf1) ;
//        printf("目录创建成功") ;
    }else if (len ==4&& args[1][0]=='-' && args[1][1]=='m' ){
        create_dir(args[3] ,args[2] ) ;
    }


    return 1;
}