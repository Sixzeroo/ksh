/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int ksh_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "ksh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("ksh");
        }
    }
    return 1;
}