#include "main.h"
/*
* main - simple shell
* Return: 0 if success, 1 if failure
*/
int main()
{
	extern char ** environ;
	char *buf = NULL, *delim = " \t\n", *cmd = NULL, *token = NULL, **params = NULL, *newline = "\n";
	size_t bufSize = 0;
	int f, i, cmdLen, inputLen, status, paramCount;

	while (1)
	{
		printf("#cisfun$ ");
		inputLen = getline(&buf, &bufSize, stdin);
		if (inputLen == -1)
			return (1);

		/* remove newline from buf */
		buf = strtok(buf, newline);

		/* length of command for malloc */
		for (cmdLen = 0; buf[cmdLen] != ' ' && buf[cmdLen]; cmdLen++);
		cmdLen += 6;
		cmd = malloc(cmdLen);
		if (!cmd)
			return (1);

		/* counts number of params */
		for (i = 0, paramCount = 0; buf[i] ; i++)
			if (buf[i] == ' ' && (!buf[i + 1] || buf[i + 1] == ' '))
				paramCount++;

		params = tokenizer(buf, params, paramCount);
		if (!params)
			return (1);
		token = strtok(buf, delim);
		strcat(cmd, "/bin/");
		strcat(cmd, token);
		f = fork();
		if (f != 0)
			wait(&status);
		else
			execve(cmd, params, environ);
	}
	/* free mallocs in create_parameter_array */
	return (0);
}

char **tokenizer(char *buf, char **params, int paramCount)
{
	char *token;
	int i;
	
	params = malloc((sizeof(char *) * paramCount) + 1);
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