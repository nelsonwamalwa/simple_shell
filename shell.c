#include "shell.h"

/**
 * displayPrompt - Display the shell prompt
 *
*/
void displayPrompt(void)
{
    printf("Shell> ");
    fflush(stdout);
}

/**
 * main - this is where the magic happens
 *
 */
int main(void)
{
	char *command = NULL;
	size_t commandSize = 0;
	char *args[MAX_ARGS];
	char *token;
	int argCount = 0;

	while (1)
	{
		ssize_t bytesRead;
		pid_t childPid;
		displayPrompt();
/* Read user input using getline */
		bytesRead = getline(&command, &commandSize, stdin);
		if (bytesRead == -1)
		{
			if (feof(stdin))
			{
/* Handle end of file (Ctrl+D) */
				printf("\nExiting shell.\n");
				break;
			}
			else
			{
				perror("Input error");
				clearerr(stdin); /* Clear the error state */
				continue;/* Continue to the next iteration */
			}
		}
/* Remove the newline character from the command */
		if (command[bytesRead - 1] == '\n')
		{
			command[bytesRead - 1] = '\0';
		}
/* Tokenize the command */
		token = strtok(command, " ");
		while (token != NULL && argCount < MAX_ARGS - 1)
		{
			args[argCount++] = token;
			token = strtok(NULL, " ");
		}
		args[argCount] = NULL;

/* Check if the command is the exit built-in */
		if (argCount > 0 && strcmp(args[0], "exit") == 0)
		{
/* Free the allocated memory for the command */
			free(command);
			exit(0);
		}
/* Check if the command is the env built-in */
		if (argCount > 0 && strcmp(args[0], "env") == 0)
		{
/* Print environment variables */
			extern char **environ;
			char **env;
			for (env = environ; *env != NULL; env++)
			{
				printf("%s\n", *env);
			}
		}
/* Check if the command is executable */

		if (args[0] != NULL)
		{
			if (access(args[0], X_OK) != -1 || access(args[0], F_OK)
			    != -1)
			{
/* Fork a child process */
				childPid = fork();
				if (childPid < 0)
				{
/* Error forking */
					perror("Fork error");
				}
				else if (childPid == 0)
				{
/* Child process */
/* Execute the command using execvp */
					execvp(args[0], args);
/* If execvp returns, an error occurred */
					perror("Command execution error");
					exit(EXIT_FAILURE);
				}
				else
				{
/* Parent process */
/* Wait for the child process to complete */
					int childStatus;
					waitpid(childPid, &childStatus, 0);
/* Check if the child process exited normally or due to a signal */
					if (WIFEXITED(childStatus))
					{
						int exitStatus = WEXITSTATUS(childStatus);
						printf("Child process exited with status %d\n", exitStatus);
					}
					else if (WIFSIGNALED(childStatus))
					{
						int terminatingSignal = WTERMSIG(childStatus);
						printf("Child process was terminated by signal %d\n", terminatingSignal);
					}
				}
			}
			else
			{
				printf("Command '%s' not found or not executable.\n", args[0]);
			}
		}
	}
/* Free the allocated memory for the command */
	free(command);
	return (0);
}
