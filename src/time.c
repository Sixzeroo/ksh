#include <time.h>

// TODO: 不使用日历时间，精度只到达秒

int ksh_execute(char **args);

int ksh_time(char **args)
{
    int len = getArgslen(args);

    char **new_args = malloc(64 * sizeof(char*));
    for(int i=0;i<len-1;++i)
        new_args[i] = args[i+1];
    // 开始时间
    time_t start_time = time(NULL);

    ksh_execute(new_args);

    time_t end_time = time(NULL);

    double useTime = difftime(end_time, start_time);

    printf("Use time : %lf\n", useTime);

    return 1;
}