#include "main.h"

int main()
{
	char *buf = NULL, *validPath = NULL;
	char **params = NULL, *newline = "\n", **paths = NULL;
	size_t bufSize = 0;
	int i, inputLen, paramCount = 0;
	char *buf_aux;

	while (1)
	{
		/* print prompt */
		if (isatty(0))
			printf("#cisfun$ ");

		/* read input from stdin to buf */
		do {
		inputLen = getline(&buf, &bufSize, stdin);
		} while (buf[0] == '\n' && inputLen == 2);

		/* if input is ENTER end loop */
		if (strcmp(buf, "\n") == 0)
		{
			free(buf);
			continue;
		}

		/* removes newline from getline buf */
		buf_aux = strtok(strdup(buf), newline);

		/* if input is "exit" exit program */
		if (strcmp(buf_aux, "exit") == 0 || inputLen == -1)
		{
			free(buf);
			exit(0);
		}
		/* parameter count */
		for (i = 0; buf_aux[i]; i++)
			if (buf_aux[i] != 32/*space*/ && (buf_aux[i + 1] == 10/*newline*/ || buf_aux[i + 1] == 32 || buf_aux[i + 1] ==  0))
				paramCount++;

		/* saves all command line arguments to *params[] */
		params = tokenizer(buf_aux, params, paramCount);
		if (!params)
		{
			printf("Error: failed to allocate memory\n");
			continue;
		}

		/* checks if bare command exists */
		if (access(params[0], F_OK) == 0)
		{
			fork_and_exec(params[0], params);
			free(buf);
			free_ap(params);
			exit(0);
		}

		/* saves PATH directories to *paths[] */
		paths = path_dirs_to_ap();
		if (!paths)
		{
			printf("Error: failed to allocate memory\n");
			free(buf);
			free_ap(params);
			continue;
		}
		
		/* checks if /pathdirs/command exists */
		validPath = check_access(paths, params[0]);
		if (!validPath)
		{
			free(buf);
			free_ap(params);
			free_ap(paths);
			continue;
		}

		/* forks and executes */
		fork_and_exec(validPath, params);

		free(buf_aux);
		free_ap(params);
		//free_ap(paths);
		free(validPath);
	}
	return (0);
}

void free_ap(char **ap)
{
	int i;

	for (i = 0; ap[i]; i++)
		free(ap[i]);
	free(ap);
}

char *check_access(char *paths[], char *command)
{
	char *fullPath = NULL;
	int i = 0, fullPathSize = 0;

	if (!paths || !command)
		return (NULL);

	for (i = 0; paths[i]; i++)
	{
		if (fullPath)
			free(fullPath);
		fullPathSize = strlen(paths[i]) + strlen(command) + 2;
		fullPath = malloc(fullPathSize);
		if (!fullPath)
			return (NULL);
		strcpy(fullPath, paths[i]);
		strcat(fullPath, "/");
		strcat(fullPath, command);
		if (access(fullPath, F_OK) == 0)
			return(fullPath);
	}
	printf("Error: command \"%s\" not found\n", command);
	return (NULL);
}

char **path_dirs_to_ap(void)
{
	char *pathDirs, *token, **paths;
	int i, pathCount = 1;

	pathDirs = strdup(getenv("PATH"));
	for (i = 0; pathDirs[i]; i++)
		if (pathDirs[i] == ':')
			pathCount++;

	paths = malloc(1024);
	if (!paths)
		return (NULL);

	token = strtok(pathDirs, ":");
	for (i = 0; i < pathCount; i++)
	{
		paths[i] = token;
		token = strtok(NULL, ":");
	}
	paths[i] = NULL;
	return (paths);
}

char **tokenizer(char *buf, char **params, int paramCount)
{
	char *token;
	int i;

	params = malloc(1024);
	if (!params)
		return (NULL);
	token = strtok(strdup(buf), " ");
	for (i = 0; i < paramCount; i++)
	{
		params[i] = token;
		token = strtok(NULL, " ");
	}
	params[i] = NULL;
	return (params);
}

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