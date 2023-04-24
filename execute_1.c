#include "shellLib.h"
/**
 * execute - execute a command with its full path variables.
 * @data: a pointer to the program's data
 * Return: 0 if successful, -1 if there was an error.
 */
int execute(program_data *data)
{
	int retval = 0, status;
	pid_t pid;

	/* Check if the program is a built-in command */
	retval = builtins_list(data);
	if (retval != -1)
		/* If it is, execute the command and return its exit status */
		return (retval);

	/* Check if the program is in the file system */
	retval = find_program(data);
	if (retval) {
		/* If not found, return an error */
		return (retval);
	}
	else {
		/* If found, fork a new process to execute the command */
		pid = fork();
		if (pid == -1) {
			/* If fork() failed, print an error and exit */
			perror(data->command_name);
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			/* Child process: execute the command */
			retval = execve(data->tokens[0], data->tokens, data->env);
			if (retval == -1)
				/* If execve() failed, print an error and exit */
				perror(data->command_name), exit(EXIT_FAILURE);
		}
		else {
			/* Parent process: wait for child to exit and check status */
			wait(&status);
			if (WIFEXITED(status))
				errno = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				errno = 128 + WTERMSIG(status);
		}
	}
	return (0);
}
