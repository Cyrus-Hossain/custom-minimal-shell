#include "shell.h"
#include "parser.h"
#include "executor.h"

void sigint_handler(int sig) {
    (void)sig;
}

void sigchld_handler(int sig) {
    int saved_errno = errno;
    pid_t pid;
    int status;
    (void)sig;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        remove_job(pid);
    }
    errno = saved_errno;
}

int main(void) {
    char *line = NULL;
    size_t len = 0;
    struct sigaction sa_int, sa_chld;
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa_chld, NULL);
    while (1) {
        printf("myshell> ");
        fflush(stdout);
        ssize_t nread = getline(&line, &len, stdin);
        if (nread == -1) {
            if (errno == EINTR) continue;
            break;
        }
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;
        Command *cmd = parse_line(line);
        if (cmd != NULL) {
            execute(cmd, line);
            free_command(cmd);
        }
    }
    free(line);
    while (job_list != NULL) {
        Job *tmp = job_list;
        job_list = job_list->next;
        free(tmp->cmdline);
        free(tmp);
    }
    return 0;
}
