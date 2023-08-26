#include "shell.h"

void displayPrompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("Shelly> ");
		fflush(stdout);
	}
}
ssize_t getCommand(char **command, size_t *commandSize)
{
	return (getline(command, commandSize, stdin));
}

void executeCommand(char **args)
{
	execvp(args[0], args);
	perror("Command execution error");
	exit(EXIT_FAILURE);
}

void waitForChildProcess(pid_t childPid)
{
	int childStatus;

	waitpid(childPid, &childStatus, 0);
	if (WIFEXITED(childStatus))
	{
		(void)WEXITSTATUS(childStatus);
	}
}

int main(void)
{
	char *command = NULL;
	size_t commandSize = 0;

	while (1)
	{
		ssize_t bytesRead;
		pid_t childPid;
		char *args[MAX_ARGS];
		int argCount = 0;
		char *token;

		displayPrompt();
		bytesRead = getCommand(&command, &commandSize);
		if (bytesRead == -1)
		{
			if (feof(stdin))
				break;
			perror("Input error");
			clearerr(stdin);
			continue;
		}
		if (command[bytesRead - 1] == '\n')
		{
			command[bytesRead - 1] = '\0';
		}
		token = strtok(command, " ");
		while (token != NULL && argCount < MAX_ARGS - 1)
		{
			args[argCount++] = token;
			token = strtok(NULL, " ");
		}
		args[argCount] = NULL;
		childPid = fork();
		if (childPid < 0)
		{
			perror("Fork error");
		}
		else if (childPid == 0)
		{
			executeCommand(args);
		}
		else
		{
			waitForChildProcess(childPid);
		}
	}
	free(command);
	return (0);
}
