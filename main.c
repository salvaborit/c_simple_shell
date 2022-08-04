#include "main.h"

int main()
{
	char *buf = NULL, *pathDirs = NULL, *validPath;
	char **params = NULL, *newline = "\n", **paths = NULL;
	size_t bufSize = 0;
	int i, cmdLen, inputLen, paramCount = 0, k;

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

		if (tokenizer(buf, params, paramCount) == 0)
			exit(0);

		pathDirs = getenv("PATH");
		if (!pathDirs)
		{
			for (i = 0; params[i]; i++)
				free(params[i]);
			free (params);
			exit(0);
		}

printf("pathDirs: %s\n", pathDirs);

		k = path_dirs_to_array(paths, pathDirs);

printf("k = %d\n", k);
		
		validPath = check_access(pathDirs, params[0]);
printf("validPath = %s\n", validPath);
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

int path_dirs_to_array(char **paths, char *pathDirs)
{
	char *token;
	int i, pathCount = 1;

	for (i = 0; pathDirs[i]; i++)
		if (pathDirs[i] == ':')
			pathCount++;


	paths = malloc(1024);
	if (!paths)
		return (0);

	token = strtok(pathDirs, ":");

	for (i = 0; i < pathCount; i++)
	{
		paths[i] = token;
printf("path[%d] = %s\n", i, paths[i]);
		token = strtok(NULL, ":");
	}
	paths[i] = NULL;
	return (1);
}

int tokenizer(char *buf, char **params, int paramCount)
{
	char *token;
	int i;

	params = malloc(1024);
	if (!params)
		return (0);
	token = strtok(buf, " ");
	for (i = 0; i < paramCount; i++)
	{
		params[i] = token;
		token = strtok(NULL, " ");
	}
	params[i] = NULL;
	return (1);
}

char *check_access(char *pathDirs, char *command)
{
	char *path, *fullPath = NULL;
	int validPath;

	if (!pathDirs || !command)
		return (NULL);
	
	path = strtok(pathDirs, ":");
	while (path)
	{
		fullPath = malloc(strlen(path) + strlen(command) + 2);
		if (!fullPath)
			return (NULL);
		strcat(fullPath, path);
		if (!fullPath)
			return (NULL);
		strcat(fullPath, "/");
		if (!fullPath)
			return (NULL);
		strcat(fullPath, command);
		if (!fullPath)
			return (NULL);
		validPath = access(fullPath, F_OK);
		if (validPath == 0)
			return(fullPath);
		free(fullPath);
		path = strtok(NULL, ":");
	}
	printf("Error: command \"%s\" not found\n", command);
	return (NULL);
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