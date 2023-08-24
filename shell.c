#include "shell.h"

/**
 * _getline - Wrapper function for the getline function.
 * @line: Pointer to the buffer that will store the line.
 * @n: Pointer to the size of the buffer.
 * @stream: Pointer to the file stream.
 *
 * Return: The number of characters read, or -1 on failure.
 */
ssize_t _getline(char **line, size_t *n, FILE *stream)
{
	return (getline(line, n, stream));
}

/**
 * parse_input - Parse input into individual arguments.
 * @input: Input string to be parsed.
 * @args: Array to store parsed arguments.
 *
 * Return: Number of arguments parsed.
 */
int parse_input(char *input, char **args)
{
	int argc = 0;
	char *token = strtok(input, " \t\n");

	while (token != NULL)
	{
		args[argc++] = token;
		token = strtok(NULL, " \t\n");
	}
	args[argc] = NULL;
	return (argc);
}

/**
 * main - Entry point of the custom shell.
 *
 * Return: Always 0.
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while (1)
	{
		printf("CustomShell$ ");
		fflush(stdout);
		read = _getline(&line, &len, stdin);
		if (read == -1)
		{
			break;
		}

		if (read > 1)
			{
			char *args[MAX_ARGS];
			int argc = parse_input(line, args);

			if (argc > 0)
			{
				if (strcmp(args[0], "cd") == 0)
				{
					if (argc != 2)
					{
						fprintf(stderr, "cd: Usage: cd <directory>\n");
					}
					else
					{
						if (chdir(args[1]) != 0)
						{
							perror("cd");
						}
					}
				}
				else if (strcmp(args[0], "exit") == 0)
				{
					exit(0);
				}
				else
				{
					pid_t pid = fork();

					if (pid == 0)
					{
						execvp(args[0], args);
						perror("execvp");
						exit(1);
					}
					else if (pid > 0)
					{
						int status;
						waitpid(pid, &status, 0);
					}
					else
					{
						perror("fork");
					}
				}
			}
		}
	}
	free(line);
	return (0);
}
