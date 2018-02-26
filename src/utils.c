
int getArgslen(char **args)
{
    int len = 0;
    char *tmp = args[0];
    while (tmp)
    {
        ++len;
        tmp = args[len];
    }
    return len;
}