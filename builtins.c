#include "builtins.h"
#include "job.h"

int is_builtin(char *cmd) {
    if (cmd == NULL) return 0;
    if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "jobs") == 0) return 1;
    return 0;
}

int execute_builtin(Command *cmd) {
    if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argv[1] == NULL) {
            char *home = getenv("HOME");
            if (home != NULL) {
                if (chdir(home) != 0) fprintf(stderr, "myshell: cd: %s\n", strerror(errno));
            } else {
                fprintf(stderr, "myshell: cd: HOME not set\n");
            }
        } else {
            if (chdir(cmd->argv[1]) != 0) fprintf(stderr, "myshell: cd: %s: %s\n", cmd->argv[1], strerror(errno));
        }
        return 0;
    } else if (strcmp(cmd->argv[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(cmd->argv[0], "jobs") == 0) {
        print_jobs();
        return 0;
    }
    return -1;
}
