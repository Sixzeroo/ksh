#include <time.h>

int ksh_date(char **args)
{
    time_t nowtime = time(NULL);
    printf("%s", asctime(gmtime(&nowtime)));
    return 1;
}