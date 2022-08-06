#include "main.h"

/**
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

/**
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