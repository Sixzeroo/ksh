#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/vfs.h>
#include <math.h>
#define SIZE1 10000
#define FN "/etc/mtab" // 文件描述 : 系统已经装载的文件系统，包括操作系统建立的虚拟文件
#define SPACE ' '
// 每块当做4096字节来模拟　

int displayapartition(char * pt,char * pt1);

int ksh_df(char **args){

    char tmpline[SIZE1];
    FILE * fp;
    char * pt1;
    char * pt2;
    char * pt3;


    if( (fp = fopen(FN,"r")) == NULL ){
        fprintf(stderr,"%s \n",strerror(errno));
        perror("ksh") ;
        return 1 ;
    }

    printf("文件系统  block总数  block已用  block未用  已用占比  挂载点  \n");

    while( fgets(tmpline, SIZE1, fp) != NULL ){
        pt1=strchr(tmpline, SPACE);
        pt2=pt1+sizeof(char);
        *pt1='\0';
        pt3=strchr(pt2,SPACE);
        *pt3='\0';
        if(strstr(tmpline,"/dev") != NULL  || strstr(tmpline,"tmpfs") != NULL ){
            displayapartition(tmpline,pt2);
        }
    }

    return 1;
}

int displayapartition(char * pt,char * pt1){

    struct statfs buf;
    statfs(pt1,&buf);
    int usage;
    usage=((buf.f_blocks-buf.f_bfree)*100/buf.f_blocks);

    printf("%s  ",pt);
    printf("%ld  ",buf.f_blocks);
    printf("%ld  ",buf.f_blocks-buf.f_bfree);
    printf("%ld  ",buf.f_bavail);
    printf("%d%%  ",usage);
    printf("%s  ",pt1);
    printf("\n");

    return 0;
}