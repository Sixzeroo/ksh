#include <string.h>

#define COMMANDNUM 21

int ksh_man(char **args)
{
    char helpText[20][2][500] = { {"cd", "Change Directory - change the current working directory to a specific Folder.\nSyntax \n      cd [Directory]"},
                                  {"help", "Display help for a built-in command"},
                                  {"exit", "Exit the shell"},
                                  {"pwd", "Print Working Directory"},
                                  {"ls", "Change Directory - change the current working directory to a specific Folder.\n"
                                                 "Syntax \n"
                                                 "      cd [Options] [Directory]"},
                                  {"cp", "Copy one or more files to another location.\n"
                                                 "\n"
                                                 "Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY.\n"
                                                 "\n"
                                                 "Syntax\n"
                                                 "      cp [options]... Source Dest"}};

    for(int i=0;i<COMMANDNUM;++i)
    {
        if(strcmp(args[1], helpText[i][0]) == 0)
        {
            printf("%s\n", helpText[i][1]);
            break;
        }
    }

    return 1;

}