#include "parser.h"

Command *parse_line(char *line) {
    if (line == NULL || line[0] == '\0') return NULL;
    char *line_dup = strdup(line);
    if (line_dup == NULL) return NULL;
    Command *head = NULL;
    Command *current = NULL;
    int arg_idx = 0;
    char *saveptr = NULL;
    char *token = strtok_r(line_dup, " \t", &saveptr);
    while (token != NULL) {
        if (head == NULL) {
            head = malloc(sizeof(Command));
            if (head == NULL) { free(line_dup); return NULL; }
            head->argv = malloc(sizeof(char*) * MAX_ARGS);
            if (head->argv == NULL) { free(head); free(line_dup); return NULL; }
            head->in_file = NULL;
            head->out_file = NULL;
            head->append = 0;
            head->background = 0;
            head->next = NULL;
            current = head;
            arg_idx = 0;
        }
        if (strcmp(token, "|") == 0) {
            if (arg_idx > 0) current->argv[arg_idx] = NULL;
            Command *new_cmd = malloc(sizeof(Command));
            if (new_cmd == NULL) { free(line_dup); return NULL; }
            new_cmd->argv = malloc(sizeof(char*) * MAX_ARGS);
            if (new_cmd->argv == NULL) { free(new_cmd); free(line_dup); return NULL; }
            new_cmd->in_file = NULL;
            new_cmd->out_file = NULL;
            new_cmd->append = 0;
            new_cmd->background = 0;
            new_cmd->next = NULL;
            current->next = new_cmd;
            current = new_cmd;
            arg_idx = 0;
        } else if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0) {
            int is_append = (strcmp(token, ">>") == 0);
            token = strtok_r(NULL, " \t", &saveptr);
            if (token != NULL && current != NULL) {
                current->out_file = strdup(token);
                current->append = is_append;
            }
        } else if (strcmp(token, "<") == 0) {
            token = strtok_r(NULL, " \t", &saveptr);
            if (token != NULL && current != NULL) {
                current->in_file = strdup(token);
            }
        } else if (strcmp(token, "&") == 0) {
            if (current != NULL) current->background = 1;
        } else {
            if (current != NULL && arg_idx < MAX_ARGS - 1) {
                current->argv[arg_idx++] = strdup(token);
            }
        }
        token = strtok_r(NULL, " \t", &saveptr);
    }
    if (current != NULL) {
        if (arg_idx > 0) current->argv[arg_idx] = NULL;
        else current->argv[0] = NULL;
    }
    free(line_dup);
    return head;
}

void free_command(Command *cmd) {
    if (cmd == NULL) return;
    if (cmd->next != NULL) free_command(cmd->next);
    if (cmd->argv != NULL) {
        int i = 0;
        while (cmd->argv[i] != NULL) {
            free(cmd->argv[i]);
            i++;
        }
        free(cmd->argv);
    }
    if (cmd->in_file != NULL) free(cmd->in_file);
    if (cmd->out_file != NULL) free(cmd->out_file);
    free(cmd);
}
