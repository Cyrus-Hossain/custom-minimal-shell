#ifndef PARSER_H
#define PARSER_H

#include "shell.h"

Command *parse_line(char *line);
void free_command(Command *cmd);

#endif
