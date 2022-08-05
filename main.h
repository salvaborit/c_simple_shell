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
char *check_access(char *paths[], char *command);
void fork_and_exec(char *command, char **params);
char *_getenv(char *name);
char **path_dirs_to_array(char *pathDirs);

#endif /* MAIN_H */