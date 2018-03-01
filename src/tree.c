
#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>

#define MAXNAME 200
void s_printf(char *filename,int depth);
void s_dirwalk(char *dirname,int depth,void (*fcn)(char *,int));
void listdirtree(char *dirname,int depth);

int ksh_tree(char **argv)
{
    /**
     * @author lx
     * tree path 显示某文件目录下的树状结构
     */
      if( getArgslen(argv)==2 )
      {
          listdirtree(argv[1],0);
      }
      else
      {
        perror("路径错误") ;
      }
    return 1;
}

void listdirtree(char *dirname,int depth)
{
    struct stat stbuf;
    if((stat(dirname,&stbuf)) == -1)
    {
        fprintf(stderr,"listdirtree:can't read file %s information!\n",dirname);
        return;
    }
    // 是文件夹
    if((stbuf.st_mode & S_IFMT) == S_IFDIR)
        s_dirwalk(dirname,depth,listdirtree);
}

void s_dirwalk(char *dirname,int depth,void (*fcn)(char *,int))
{
    char name[MAXNAME];
    struct dirent *fip;
    DIR *dp;
    if((dp = opendir(dirname)) == NULL)
    {
        fprintf(stderr,"s_dirwalk:can't open %s\n",dirname);
        return;
    }
    // 循环读取 dp 目录下的 子目录
    while((fip = readdir(dp)) != NULL)
    {
        if(strcmp(fip->d_name,".") == 0 || strcmp(fip->d_name,"..") == 0)
            continue;
        if(strlen(dirname) + strlen(fip->d_name) + 2 > sizeof(name))
        {
            fprintf(stderr,"s_dirwalk:%s/%s is too long!\n",dirname,fip->d_name);
            return;
        }
        else
        {
            s_printf(fip->d_name,depth);
            sprintf(name,"%s/%s",dirname,fip->d_name);
            (*fcn)(name,depth + 1);
        }
    }
    closedir(dp);
}

void s_printf(char *filename,int depth)
{
    while(depth-- > 0)
        printf("|  ");
    printf("|--");
    printf("%s\n",filename);
}
