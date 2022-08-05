#include "main.h"

/*
* main - simple shell program
* Return: 0 on success
*/
int main(void)
{
	extern char **environ;
	char *buf = NULL, *validPath = NULL, *buf_aux, *newline = "\n";
	char **params = NULL, **paths = NULL;
	size_t bufSize = 0;
	int inputLen = 0, i;

	while (1)
	{
		if (isatty(0))/* print prompt */
			printf("#cisfun$ ");
		
		inputLen = getline(&buf, &bufSize, stdin);/* stdin to buf */
		if (inputLen == -1)
		{
			if (buf)
			{
				free(buf);
				buf = NULL;
			}
			exit(0);
		}
		
		if (strcmp(buf, "\n") == 0)
		{/* if input is ENTER end loop */
			free(buf);
			buf = NULL;
			continue;
		}
		
		buf_aux = strdup(buf); /* removes newline from getline buf */
		strtok(buf_aux, newline);
		free(buf);
		buf = NULL;

		if (strcmp(buf_aux, "exit") == 0 || inputLen == -1)
		{/* if input is "exit" exit program */

			free(buf_aux);
			exit(0);
		}

		if (strcmp(buf_aux, "env") == 0)
		{
			for (i = 0; environ[i]; i++)
				puts(environ[i]);
			free(buf_aux);
			continue;
		}
		
		params = tokenizer(buf_aux);/* CL args to *params[] */
		
		if (!params || !params[0])
		{/* caso borde */
			free_ap(params);
			continue;
		}
		
		if (access(params[0], F_OK) == 0)
		{/* checks if bare command exists */
			fork_and_exec(params[0], params);
			free_ap(params);
			continue;
		}
		
		paths = path_dirs_to_ap();/* saves PATH directories to *paths[] */
		if (!paths)
		{
			free_ap(params);
			continue;
		}
		
		validPath = check_access(paths, params[0]);
		if (!validPath)
		{/* checks if /pathdirs/command exists */
			free_ap(params);
			free_ap(paths);
			continue;
		}
		
		fork_and_exec(validPath, params);/* forks and executes */
		free_ap(paths);
		free_ap(params);
		free(validPath);
	}
	return (0);
}

/*
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

/*
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

/*
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

/*
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

/*
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

/*
* _getenv - gets value of environment variable
* @name: name of environment variable to retrieve value
* Return: value of environment variable
*/
char *_getenv(char *name)
{
	extern char **environ;
	char *env, *value;
	int i;

	for (i = 0; environ[i]; i++)
	{
		env = malloc(strlen(environ[i]) + 1);
		if (!env)
			return (NULL);
		env = strdup(environ[i]);
		value = strtok(env, "=");
		if (strcmp(value, name) == 0)
			value = strtok(NULL, "=");
		free(env);
	}
	return (value);
}