#include<stdio.h>
#include<pwd.h>
#include<sys/types.h>
#include<unistd.h>
int ksh_whoami(char **args)
{
    /**
     * @author lx
     * 查看当前系统登录的用户名
     */
    uid_t id;
    struct passwd * pa;
    id=geteuid();
    pa=getpwuid(id);
    printf("%s\n",pa->pw_name);
    return 1;
}