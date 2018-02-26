#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <utmp.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

int ksh_who(char **args)
{
    struct utmp *um;

    char timebuf[24];

    printf("%-12s%-12s%-20.20s  %s\n", "NAME", "LINE", "TIME", "COMMENT");

    while ((um = getutent()))
    {
        if(um->ut_type != USER_PROCESS)
            continue;
        time_t tm;
        strftime(timebuf, 24, "%F %R", localtime(&tm));
        printf("%-12s%-12s%-20.20s  (%s)\n", um->ut_user, um->ut_line, timebuf, um->ut_host);
    }

    endutent();
    return 1;
}