#include "main.h"

/**
* free_ap - frees an array of pointers
* @ap: array of pointer
*/
void free_ap(char **ap)
{
	int i;

	for (i = 0; ap[i]; i++)
		free(ap[i]);
	free(ap);
	ap = NULL;
}

/**
* check_access - checks if command exists in PATH env var directories
* @paths: array of pointers that contains PATH directories
* @command: first argument passed to command line (command)
* Return: full path of command executable if success, NULL on failure
*/
char *check_access(char *paths[], char *command)
{
	char *fullPath = NULL;
	int i = 0, fullPathSize = 0;

	if (!paths || !command)
		return (NULL);

	for (i = 0; paths[i]; i++)
	{
		fullPathSize = strlen(paths[i]) + strlen(command) + 2;
		fullPath = malloc(fullPathSize);
		if (!fullPath)
			return (NULL);
		strcpy(fullPath, paths[i]);
		strcat(fullPath, "/");
		strcat(fullPath, command);
		if (access(fullPath, F_OK) == 0)
			return (fullPath);
		free(fullPath);
	}
	printf("./hsh: 1: %s: not found\n", command);
	return (NULL);
}

/**
* path_dirs_to_ap - creates an array of pointers to store all PATH dirs
* Return: array of pointers containing all PATH dirs, NULL on failure
*/
char **path_dirs_to_ap(void)
{
	char *pathDirs, *token, **paths, *aux;
	int i;
	pathDirs = strdup(getenv("PATH"));

	paths = malloc(1024);

	if (!paths)
		return (NULL);
	token = strdup(pathDirs);
	strtok(token, ":");
	aux = token;
	for (i = 0; aux; i++)
	{
		paths[i] = strdup(aux);
		aux = strtok(NULL, ":");
	}
	free(pathDirs);
	free(token);
	paths[i] = NULL;
	return (paths);
}

/**
* tokenizer - creates an array of pointers to store all command line arguments
* @buf: string that contains all command line arguments
* Return: array of pointers containing all command line arguments, NULL on failure
*/
char **tokenizer(char *buf)
{
	char *token = NULL, **params = NULL;
	int i;

	params = malloc(1024);
	if (!params)
		return (NULL);
	token = strtok(buf, " \n\t");
	for (i = 0; token && token[0]; i++)
	{
		params[i] = strdup(token);
		token = strtok(NULL, " \n\t");
	}
	params[i] = NULL;
	free(buf);
	return (params);
}

/**
* fork_and_exec - self explanatory
* @command: first command line argument, command
* @params: array of pointers to all command line arguments
*/
void fork_and_exec(char *command, char **params)
{
	extern char **environ;
	pid_t id;
	int status;

	id = fork();
	if (id == 0)
		execve(command, params, environ);
	else
		wait(&status);
	return;
}