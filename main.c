#include "main.h"

int main()
{
	char *buf = NULL, *pathDirs = NULL, *validPath = NULL;
	char **params = NULL, *newline = "\n", **paths = NULL;
	size_t bufSize = 0;
	int i, cmdLen, inputLen, paramCount = 0;

	while (1)
	{
		if (isatty(0) == 1)
			printf("#cisfun$ ");

		do {
		inputLen = getline(&buf, &bufSize, stdin);
		} while (buf[0] == '\n' && inputLen == 2);

		if (strcmp(buf, "\n") == 0)
			continue;

		buf = strtok(buf, newline);
		if (strcmp(buf, "exit") == 0 || inputLen == -1)
			exit(0);
	
		for (cmdLen = 0; buf[cmdLen] != ' ' && buf[cmdLen]; cmdLen++);

		for (i = 0; buf[i]; i++)
			if (buf[i] != 32/*space*/ && (buf[i + 1] == 10/*newline*/ || buf[i + 1] == 32 || buf[i + 1] ==  0))
				paramCount++;

		params = tokenizer(buf, params, paramCount);
		if (!params)
			exit(0);

		pathDirs = getenv("PATH");
		if (!pathDirs)
		{
			for (i = 0; params[i]; i++)
				free(params[i]);
			free (params);
			exit(0);
		}

		paths = malloc(1024);
		if (!paths)
		{
			for (i = 0; params[i]; i++)
				free(params[i]);
			free (params);
			exit(0);
		}
		paths = path_dirs_to_array(pathDirs);
		
		validPath = malloc(1024);
		if (!validPath)
			exit(0);
		validPath = check_access(paths, params[0]);

		if (validPath)
			fork_and_exec(validPath, params);
		else
		{
			for (i = 0; params[i]; i++)
				free(params[i]);
			free (params);

			exit(0);
		}
	}
	return (0);
}

char *check_access(char *paths[], char *command)
{
	char *fullPath = NULL;
	int i;

	if (!paths || !command)
		return (NULL);

	for (i = 0; paths[i]; i++)
	{
		fullPath = malloc(strlen(paths[i]) + strlen(command) + 2);
		if (!fullPath)
			return (NULL);
		strcat(fullPath, paths[i]);
		strcat(fullPath, "/");
		strcat(fullPath, command);
		if (access(fullPath, F_OK) == 0)
			return(fullPath);
		free(fullPath);
	}
	printf("Error: hellocommand \"%s\" not found\n", command);
	return (NULL);
}


char **path_dirs_to_array(char *pathDirs)
{
	char *token, **paths;
	int i, pathCount = 1;

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
	token = strtok(buf, " ");
	for (i = 0; i < paramCount; i++)
	{
		params[i] = token;
		token = strtok(NULL, " ");
	}
	params[i] = NULL;
	return (params);
}

pid_t fork_and_exec(char *command, char **params)
{
	extern char **environ;
	pid_t id;
	int status;

	id = fork();
	if (id == 0)
		execve(command, params, environ);
	else
		wait(&status);
	return (id);
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