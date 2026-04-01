#ifndef BUILTINS_H
#define BUILTINS_H

#include "shell.h"

int is_builtin(char *cmd);
int execute_builtin(Command *cmd);

#endif
