#include "main.h"
/*
* main - simple shell
* Return: 0 if success, 1 if failure
*/
int main()
{
	char *buf = NULL, *pathDirs = NULL, *validPath;
	char **params = NULL, *newline = "\n";
	size_t bufSize = 0;
	int i, cmdLen, inputLen, paramCount = 0;

	while (1)
	{
		if(isatty(0) == 1)
			printf("#cisfun$ ");

		do {
		inputLen = getline(&buf, &bufSize, stdin);
		} while (buf[0] == '\n' && inputLen == 2);
		if (inputLen == -1)
			exit(EXIT_SUCCESS);

		buf = strtok(buf, newline);

		for (cmdLen = 0; buf[cmdLen] != ' ' && buf[cmdLen]; cmdLen++);

		for (i = 0; buf[i]; i++)
		{
			if (buf[i] != 32/*space*/ && (buf[i + 1] == 10/*newline*/ || buf[i + 1] == 32 || buf[i + 1] ==  0))
				paramCount++;
		}

		params = tokenizer(buf, params, paramCount);
		if (!params)
			return (0);
		pathDirs = getenv("PATH");
		validPath = check_access(pathDirs, params[0]);
		if (validPath)
			fork_and_exec(validPath, params);
		else
			exit(EXIT_SUCCESS);
	}
	return (0);
}

pid_t fork_and_exec(char *command, char **params)
{
	extern char **environ;
	pid_t id;
	int status;

	id = fork();
	if (id == 0)
	{
		execve(command, params, environ);
	}
	else
		wait(&status);
	return (id);
}

/*
* check_access: checks if file in path exists
* Return: 1 if found, 0 if not found
*/
char *check_access(char *pathDirs, char *command)
{
	char *path, *fullPath;
	int validPath;
	
	path = strtok(pathDirs, ":");
	while (path)
	{
		fullPath = malloc(strlen(path) + strlen(command) + 2);
		strcat(fullPath, path);
		strcat(fullPath, "/");
		strcat(fullPath, command);
		validPath = access(fullPath, F_OK);
		if (validPath == 0)
			return(fullPath);
		free(fullPath);
		path = strtok(NULL, ":");
	}
	printf("Error: command \"%s\" not found\n", command);
	return (0);
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

char **tokenizer(char *buf, char **params, int paramCount)
{
	char *token;
	int i;

	params = malloc(sizeof(char *) * paramCount);
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