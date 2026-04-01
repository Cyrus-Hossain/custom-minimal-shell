#include "executor.h"
#include "builtins.h"

static pid_t launch_pipeline(Command *cmd) {
    if (cmd == NULL || cmd->argv[0] == NULL) return -1;
    int prev_fd = -1;
    pid_t last_pid = -1;
    Command *c = cmd;
    while (c != NULL) {
        int pipe_fds[2] = {-1, -1};
        if (c->next != NULL) {
            if (pipe(pipe_fds) == -1) {
                fprintf(stderr, "myshell: pipe: %s\n", strerror(errno));
                return -1;
            }
        }
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "myshell: fork: %s\n", strerror(errno));
            if (pipe_fds[0] != -1) close(pipe_fds[0]);
            if (pipe_fds[1] != -1) close(pipe_fds[1]);
            return -1;
        }
        if (pid == 0) {
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (c->next != NULL) {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
                close(pipe_fds[0]);
            }
            if (c->in_file != NULL) {
                int fd = open(c->in_file, O_RDONLY);
                if (fd == -1) {
                    fprintf(stderr, "myshell: %s: %s\n", c->in_file, strerror(errno));
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (c->out_file != NULL) {
                int flags = O_WRONLY | O_CREAT | (c->append ? O_APPEND : O_TRUNC);
                int fd = open(c->out_file, flags, 0666);
                if (fd == -1) {
                    fprintf(stderr, "myshell: %s: %s\n", c->out_file, strerror(errno));
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            execvp(c->argv[0], c->argv);
            fprintf(stderr, "myshell: %s: %s\n", c->argv[0], strerror(errno));
            exit(127);
        }
        if (prev_fd != -1) close(prev_fd);
        if (c->next != NULL) {
            close(pipe_fds[1]);
            prev_fd = pipe_fds[0];
        }
        last_pid = pid;
        c = c->next;
    }
    return last_pid;
}

int execute(Command *cmd, char *original_line) {
    if (cmd == NULL || cmd->argv[0] == NULL) return 0;
    if (cmd->next == NULL && is_builtin(cmd->argv[0])) {
        return execute_builtin(cmd);
    }
    pid_t last_pid = launch_pipeline(cmd);
    if (last_pid > 0) {
        Command *last = cmd;
        while (last->next != NULL) last = last->next;
        if (last->background) {
            add_job(last_pid, original_line);
        } else {
            waitpid(last_pid, NULL, 0);
        }
    }
    return 0;
}
