#include "shell.h"

/**
 * displayPrompt - Displays the shell prompt "Shelly> " if input is from a term
 *inal
 * None
 * Return: None
 */
void displayPrompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("Shelly> ");
		fflush(stdout);
	}
}

/**
 * getCommand - Reads a line from standard input into the provided buffer.
 *
 * @command: Pointer to the buffer where the command will be stored.
 * @commandSize: Pointer to the size of the command buffer.
 * Return: The number of characters read, or -1 on failure.
 */
ssize_t getCommand(char **command, size_t *commandSize)
{
	return (getline(command, commandSize, stdin));
}

/**
 * executeCommand - Executes a command using the execvp system call.
 *
 * @args: Null-terminated array of command arguments.
 * Return: This function does not return on success, or exits with an error mes
 * sage on failure.
 */
void executeCommand(char **args)
{
	execvp(args[0], args);
	perror("Command execution error");
	exit(EXIT_FAILURE);
}

/**
 * waitForChildProcess - Waits for a child process to complete.
 *
 * @childPid: The process ID of the child process to wait for.
 * Return: None
 */
void waitForChildProcess(pid_t childPid)
{
	int childStatus;

	waitpid(childPid, &childStatus, 0);
	if (WIFEXITED(childStatus))
	{
		(void)WEXITSTATUS(childStatus);
	}
}

/**
 * main - Entry point of the program.
 *
 * This is the main function where the program's execution begins. It serves as
 * the
 * entry point for the program's logic and control flow. Within this function,
 * you can
 * call other functions, perform initialization, and manage the overall executi
 * on of
 * the program.
 *
 * @void: This function accepts no arguments.
 * Return: The exit status of the program. Typically, 0 indicates successful
 * execution,and non-zero values are used to indicate errors or
 * abnormal termination.
 */
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
