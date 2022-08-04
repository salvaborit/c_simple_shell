#include "main.h"

int main()
{
	char *buf = NULL, *pathDirs = NULL, *validPath;
	char **params = NULL, *newline = "\n";
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
		{
			free(buf);
			exit(EXIT_SUCCESS);
		}
	
		for (cmdLen = 0; buf[cmdLen] != ' ' && buf[cmdLen]; cmdLen++);

		for (i = 0; buf[i]; i++)
		{
			if (buf[i] != 32/*space*/ && (buf[i + 1] == 10/*newline*/ || buf[i + 1] == 32 || buf[i + 1] ==  0))
				paramCount++;
		}

		params = tokenizer(buf, params, paramCount);
		pathDirs = getenv("PATH");
		if (!pathDirs)
		{
			free(pathDirs);
			for(i = 0; params[i]; i++)
				free(params[i]);
			free(params);
			free(buf);
		}
		validPath = check_access(pathDirs, params[0]);
		if (validPath)
			fork_and_exec(validPath, params);
		else
		{
			free(validPath);
			free(pathDirs);
			for(i = 0; params[i]; i++)
				free(params[i]);
			free(params);
			free(buf);
			exit(EXIT_SUCCESS);
		}
	}
	return (0);
}

char *check_access(char *pathDirs, char *command)
{
	char *path, *fullPath = NULL;
	int validPath;
	
	path = strtok(pathDirs, ":");
	while (path)
	{
		fullPath = malloc(strlen(path) + strlen(command) + 2);
		strncpy(fullPath, path, strlen(path));
		strcat(fullPath, "/");
		strcat(fullPath, command);
		validPath = access(fullPath, F_OK);
		if (validPath == 0)
			return(fullPath);
		free(fullPath);
		path = strtok(NULL, ":");
	}
	printf("Error: command \"%s\" not found\n", command);
	free(path);
	return (0);
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
		env = strdup(environ[i]);
		value = strtok(env, "=");
		if (strcmp(value, name) == 0)
		{
			value = strtok(NULL, "=");
		}
		free(env);
	}
	return (value);
}

char **tokenizer(char *buf, char *params[], int paramCount)
{
	char *token;
	int i;

	params = malloc(1024);
	if (!params)
		return (params);
	token = strtok(buf, " ");
	for (i = 0; i < paramCount; i++)
	{
		params[i] = token;
		token = strtok(NULL, " ");
	}
	params[i] = NULL;
	return (params);
}