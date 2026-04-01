#include "job.h"

Job *job_list = NULL;

void add_job(pid_t pid, char *cmdline) {
    Job *j = malloc(sizeof(Job));
    if (j == NULL) return;
    j->pid = pid;
    j->cmdline = strdup(cmdline);
    j->next = job_list;
    job_list = j;
}

void remove_job(pid_t pid) {
    Job **curr = &job_list;
    while (*curr != NULL) {
        if ((*curr)->pid == pid) {
            Job *tmp = *curr;
            *curr = tmp->next;
            free(tmp->cmdline);
            free(tmp);
            return;
        }
        curr = &(*curr)->next;
    }
}

void print_jobs(void) {
    Job *j = job_list;
    while (j != NULL) {
        printf("[%d] Running %s\n", j->pid, j->cmdline);
        j = j->next;
    }
}
