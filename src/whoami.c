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
    /**
     * pwsswd 的 结构
     * char *pw_name;		 Username.
       char *pw_passwd;		 Password.
       __uid_t pw_uid;		 User ID.
       __gid_t pw_gid;		 Group ID.
       char *pw_gecos;		 Real name.
       char *pw_dir;			 Home directory.
       char *pw_shell;		 Shell program.
     */
    id=geteuid(); // 返回当前进程的用户ID
    pa=getpwuid(id);
    printf("%s\n",pa->pw_name); // 返回用户名
    return 1;
}