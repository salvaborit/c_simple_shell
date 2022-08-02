#include "main.h"
/*
* main - simple shell
* Return: 0 if success, 1 if failure
*/
int main()
{
	char *buf = NULL, *delim = " \t\n", *cmd = NULL, *token = NULL, **params = NULL, *newline = "\n";
	size_t bufSize = 0;
	int i, cmdLen, inputLen, paramCount, validPath;

	while (1)
	{
		if(isatty(0) == 1)
			printf("#cisfun$ ");

		do {
		inputLen = getline(&buf, &bufSize, stdin);
		} while (buf[0] == '\n' && inputLen > 1);
		if (inputLen == -1)
			exit(EXIT_SUCCESS);

		/* remove newline from buf */
		buf = strtok(buf, newline);

		/* length of command in cmdLen for cmd malloc*/
		for (cmdLen = 0; buf[cmdLen] != ' ' && buf[cmdLen]; cmdLen++);
		cmdLen += 6;
		cmd = malloc(cmdLen);
		if (!cmd)
			return (1);

		/* put number of parameters in paramCount */
		for (i = 0, paramCount = 0; buf[i] ; i++)
			if (buf[i] != ' ' && (buf[i + 1] == '\n' || buf[i + 1] == ' '))
				paramCount++;

		params = tokenizer(buf, params, paramCount);
		if (!params)
			return (1);
		token = strtok(buf, delim);
		if (access(token, F_OK) != 0)
			strcat(cmd, "/bin/");
		strcat(cmd, token);
		validPath = check_access(cmd, token);
		if (validPath)
			fork_and_exec(cmd, params);
	}
	free(buf);
	free(cmd);
	return (0);
}

void fork_and_exec(char *cmd, char **params)
{
	extern char **environ;
	pid_t id;
	int status;

	id = fork();
	if (id == 0)
		execve(cmd, params, environ);
	else
		wait(&status);
}

int check_access(char *path, char *token)
{
	if (access(path, F_OK) == 0)
		return (1);
	printf("Error: command \"%s\" not found\n", token);
	return (0);
}

char **tokenizer(char *buf, char **params, int paramCount)
{
	char *token;
	int i;

	params = malloc((8 * paramCount) + 1);
	if (!params)
		return (params);
	token = strtok(buf, " ");
	for (i = 0; i <= paramCount; i++)
	{
		params[i] = token;
		token = strtok(NULL, " ");
	}
	params[i] = NULL;
	return (params);
}