#ifndef JOB_H
#define JOB_H

#include "shell.h"

void add_job(pid_t pid, char *cmdline);
void remove_job(pid_t pid);
void print_jobs(void);

#endif
