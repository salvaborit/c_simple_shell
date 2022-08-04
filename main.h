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

int tokenizer(char *buf, char **params, int paramCount);
char *check_access(char *path, char *token);
pid_t fork_and_exec(char *cmd, char **params);
char *_getenv(char *name);
int path_dirs_to_array(char **paths, char *pathDirs);

#endif /* MAIN_H */