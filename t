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