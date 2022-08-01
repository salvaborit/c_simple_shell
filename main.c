#include "main.h"
/*
* main - main shell function
* Return: 0 if success, 1 if failure
*/
int main()
{
	char *buf = NULL, *delim = " \t\n", *cmd = NULL, *token = NULL, **params = NULL, *newline = "\n";
	size_t bufSize = 0;
	int f, i, cmdLen, inputLen, status, spaces;

	while (1)
	{
		printf("#cisfun$ ");
		inputLen = getline(&buf, &bufSize, stdin);
		if (inputLen == -1)
			return (1);
		buf = strtok(buf, newline);
		for (cmdLen = 0; buf[cmdLen] != ' ' && buf[cmdLen]; cmdLen++);
		cmdLen += 6;
		cmd = malloc(cmdLen);
		if (!cmd)
			return (1);
		for (i = 0, spaces = 0; buf[i]; i++)
			if (buf[i] == ' ')
				spaces++;
		params = create_parameter_array(buf, params, spaces);
		if (!params)
			return (1);
		i = 0;
		token = strtok(buf, delim);
		strcat(cmd, "/bin/");
		strcat(cmd, token);
		f = fork();
		if (f != 0)
			wait(&status);
		else
			execve(cmd, params, NULL);
	}
	/* free mallocs in create_parameter_array */
	return (0);
}

char **create_parameter_array(char *buf, char **params, int spaces)
{
	char *token;
	int i;
	
	params = malloc((sizeof(char *) * spaces) + 1);
	if (!params)
		return (params);
	token = strtok(buf, " ");
	for (i = 0; i <= spaces; i++)
	{
		params[i] = token;
		token = strtok(NULL, " ");
	}
	params[i] = NULL;
	i = 0;
	return (params);
}
