#define _POSIX_C_SOURCE 200809L
#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAX_ARGS 32

typedef struct Command {
    char **argv;
    char *in_file;
    char *out_file;
    int append;
    int background;
    struct Command *next;
} Command;

typedef struct Job {
    pid_t pid;
    char *cmdline;
    struct Job *next;
} Job;

extern Job *job_list;

Command *parse_line(char *line);
void free_command(Command *cmd);
int execute(Command *cmd, char *original_line);
int is_builtin(char *cmd);
int execute_builtin(Command *cmd);
void add_job(pid_t pid, char *cmdline);
void remove_job(pid_t pid);
void print_jobs(void);
void sigint_handler(int sig);
void sigchld_handler(int sig);

#endif
