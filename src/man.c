#include <string.h>

#define COMMANDNUM 25

int ksh_man(char **args)
{
    char helpText[COMMANDNUM][2][500] = { {"cd", "Change Directory - change the current working directory to a specific Folder.\nSyntax \n cd [Directory]"},
                                  {"help", "Display help for a built-in command"},
                                  {"exit", "Exit the shell"},
                                  {"pwd", "Print Working Directory"},
                                  {"ls", "Syntax\n"
                                                 "      ls [Options]... [File]...\n"
                                                 "\n"
                                                 "Key\n"
                                                 "\n"
                                                 "  -a                  List all entries including those starting with a dot .\n"
                                                 "  -l                  Use a long listing format"},
                                  {"cp", "Copy one or more files to another location.\n"
                                                 "\n"
                                                 "Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY.\n"
                                                 "\n"
                                                 "Syntax\n"
                                                 "      cp [options]... Source Dest"
                                                 "\n"
                                                 "Options\n"
                                                 "\n"
                                                 "-r       copy directory "},
                                  {"mv", "Move or rename files or directories.\n"
                                                 "\n"
                                                 "SYNTAX\n"
                                                 "      mv [options]... Source Dest"},
                                  {"rm", "Remove files (delete/unlink)\n"
                                                 "\n"
                                                 "Syntax\n"
                                                 "      rm [options]... file...\n"
                                                 "\n"
                                                 "Options\n"
                                                 "\n"
                                                 "-r       Remove directories and their contents recursively "},
                                  {"time", "Measure the running time of a program.\n"
                                                   "\n"
                                                   "Syntax\n"
                                                   "      time command [arg...]"},
                                  {"who", "Print who is currently logged in\n"},
                                  {"date", "Display the date.\n"},
                                  {"touch", "Create a file\n"},
                                  {"grep", "Search file(s) for lines that match a given pattern\n"},
                                  {"last", "show listing of last logged in users \n"},
                                  {"sort", "Sort text files\n"
                                                   "\n"
                                                   "Syntax\n"
                                                   "      sort [file...]"},
                                  {"mkdir","Create new folder\n"
                                                        "\n"
                                                        "Syntax\n"
                                                        "   mkdir [Options] folder..."
                                                        "Options\n"
                                                        "  -p, --parents     no error if existing, make parent directories as needed"},
                                  {"cat","Concatenate and print (display) the content of files.\n"
                                                 "\n"
                                                 "Syntax\n"
                                                 "      cat [Options] [File]...\n"
                                                 "\n"
                                                 "Concatenate FILE(s), or standard input, to standard output."
                                                 "-n, --number             number all output lines"},
                                  {"df","Disk Free - display free disk space.\n"
                                                "With no arguments, `df' reports the space used and available on all currently mounted filesystems (of all types). Otherwise, `df' reports on the filesystem containing each argument file."
                                                },
                                  {"kill","Kill a process by specifying its PID, either via a signal or forced termination.\n"
                                                  "\n"
                                                  "Syntax\n"
                                                  "      kill  [-n signum] jobspec or pid\n"
                                                  "\n"
                                                  "Key\n"
                                                  "   -n   Send a specific signal number\n"
                                                  "Send a signal specified by sigspec or signum to the process named by job specification jobspec or process ID pid. "},
                                  {"tree","show the path tree of the path\n"
                                                "\n"
                                                "Syntax\n"
                                                "       tree pathname \n"},
                                  {"head","Output the first part of files, prints the first part (10 lines by default) of each file.\n"
                                                  "\n"
                                                  "SYNTAX\n"
                                                  "      head [options]... [file]...\n"
                                                  "\n"
                                                  "Options:\n"
                                                  "  -n N\n"
                                                  "  --lines=N\n"
                                                  "       Output the first N lines."},
                                  {"ps","Process status, information about processes running in memory."},
                                  {"whoami","Print the current user id and name."}

    };

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