#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>
#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

// TODO: 测试

static char utmpbuf[NRECS*UTSIZE];
static int num_recs;
static int cur_rec;
static int fdWtmp = -1;
struct utmp_list
{
    struct utmp value;
    struct utmp_list * next;
};
int wtmp_open(char * filename)
{
    fdWtmp = open(filename, O_RDONLY);
    cur_rec = num_recs = 0;
    return fdWtmp;
}
int wtmp_reload(void)
{
    int amt_read;
    amt_read = read(fdWtmp, utmpbuf, NRECS * UTSIZE);
    num_recs = amt_read/UTSIZE;
    cur_rec = 0;
    return num_recs;
}
struct utmp * wtmp_next(void)
{
    struct utmp * recp;
    if(fdWtmp == -1)
        return NULLUT;
    if(cur_rec == num_recs && wtmp_reload() == 0)
        return NULLUT;
    recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];
    cur_rec++;
    return recp;
}
void wtmp_close(void)
{
    if(fdWtmp != -1)
        close(fdWtmp);
}
struct utmp_list * st_list_init(void)
{
    struct utmp_list * list = (struct utmp_list *)malloc(sizeof(struct utmp_list));
    if(!list)
    {
        printf("struct utmp_list malloc failed\n");
        return NULL;
    }
    memset(list, 0 , sizeof(struct utmp_list));
    return list;
}
struct utmp * st_utmp_init(void)
{
    struct utmp * stUTMP = (struct utmp *)malloc(sizeof(struct utmp));
    if(!stUTMP)
    {
        printf("struct utmp malloc failed\n");
        return NULL;
    }
    memset(stUTMP, 0 , sizeof(struct utmp));
    return stUTMP;
}
void list_insert(struct utmp_list * list, struct utmp * value)
{
    if(!list)
        return;
    struct utmp_list *pList = list;
    while(pList->next)
        pList = pList->next;
    struct utmp_list * node = (struct utmp_list *)malloc(sizeof(struct utmp_list));
    if(!node)
    {
        printf("malloc struct utmp_list failed\n");
        return;
    }
    if(value)
        memcpy(&node->value, value, sizeof(struct utmp));
    node->next = NULL;
    pList->next = node;
}
int list_get_ele_and_del(struct utmp_list * list, char * value, struct utmp * retUtmp)
{
    if(!list)
    {
        printf("List is empty, insert error\n");
        return -1;
    }
    struct utmp_list * pList = list;
    struct utmp_list * beforeNode = list;
    while(pList)
    {
        if(0 == strcmp(value, pList->value.ut_line))
        {
            beforeNode->next = pList->next;
            if(retUtmp)
                memcpy(retUtmp, &pList->value, sizeof(struct utmp));
            free(pList);
            return 0;
        }
        else
        {
            beforeNode = pList;
            pList = pList->next;
        }
    }
    return -1;
}
void show_end_time(long timeval)
{
    struct tm *t;
    char tt[12] = {0};
    t = localtime(&timeval);
    strftime(tt, 100, "%R", t);
    printf("%s", tt);
}
void show_start_time(long timeval)
{
    struct tm *t;
    char tt[32] = {0};
    t = localtime(&timeval);
    strftime(tt, 100, "%a %b %e %R", t);
    printf("%s", tt);
    printf(" - ");
}
void show_base_info(struct utmp * uBuf)
{
    printf("%-9.8s", uBuf->ut_name);
    if(uBuf->ut_type == BOOT_TIME)
        printf("%-13s", "system boot");
    else
        printf("%-13.8s", uBuf->ut_line);
    printf("%-17.16s", uBuf->ut_host);
}

int ksh_last(char **args)
{
    int ret = -2;
    struct utmp *utbufp, *wtmp_next();
    if(wtmp_open(WTMP_FILE) == -1)
    {
        printf("open WTMP_FILE file error\n");
        exit(1);
    }
    struct utmp_list * normalList = st_list_init();
    struct utmp_list * deadList = st_list_init();
    struct utmp * nodeUTMP = st_utmp_init();
    while((utbufp = wtmp_next()) != ((struct utmp *)NULL))
    {
        if(utbufp->ut_type == USER_PROCESS || utbufp->ut_type == BOOT_TIME)
            list_insert(normalList, utbufp);
        else if(utbufp->ut_type == DEAD_PROCESS)
            list_insert(deadList, utbufp);
    }
    normalList = normalList->next;
    while(normalList)
    {
        memset(nodeUTMP, 0, sizeof(struct utmp));
        ret = list_get_ele_and_del(deadList, normalList->value.ut_line, nodeUTMP);
        show_base_info(&normalList->value);
        show_start_time(normalList->value.ut_time);
        if(deadList != NULL && ret != -1)
            show_end_time(nodeUTMP->ut_time);
        else if(ret == -1 && normalList->value.ut_type == USER_PROCESS)
            printf("still logged in");
        else if(ret == -1 && normalList->value.ut_type == BOOT_TIME)
            printf("system boot");
        normalList = normalList->next;
        printf("\n");
    }
    wtmp_close();
    return 1;
}