#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

char **tokenizer(char *buf, char **params, int paramCount);
int check_access(char *path, char *token);
void fork_and_exec(char *cmd, char **params);

#endif /* MAIN_H */