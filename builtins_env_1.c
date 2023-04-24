#include "shellLib.h"

/**
 * builtin_env - displays the shell's environment variables
 * @data: program data structure
 * Return: 0 if successful, or an error code if specified in the arguments
 */
int builtin_env(program_data *data)
{
	int i;
	char var_name[50] = {'\0'};
	char *var_copy = NULL;

	/* if no arguments */
	if (data->tokens[1] == NULL)
		print_environ(data);
	else
	{
		for (i = 0; data->tokens[1][i]; i++)
		{
			/* checks if a character '=' exists */
			if (data->tokens[1][i] == '=')
			{
				/* checks if a variable with the same name exists and changes its value temporarily */
				var_copy = str_duplicate(env_get_key(var_name, data));
				if (var_copy != NULL)
					env_set_key(var_name, data->tokens[1] + i + 1, data);

				/* print the environment */
				print_environ(data);
				if (env_get_key(var_name, data) == NULL)
				{
					/* print the variable if it does not exist in the environment */
					_print(data->tokens[1]);
					_print("\n");
				}
				else
				{
					/* returns the old value of the variable */
					env_set_key(var_name, var_copy, data);
					free(var_copy);
				}
				return (0);
			}
			var_name[i] = data->tokens[1][i];
		}
		errno = 2;
		perror(data->command_name);
		errno = 127;
	}
	return (0);
}

/**
 * builtin_set_env - sets the value of an environment variable
 * @data: program data structure
 * Return: 0 if successful, or an error code if specified in the arguments
 */
int builtin_set_env(program_data *data)
{
	/* validate arguments */
	if (data->tokens[1] == NULL || data->tokens[2] == NULL)
		return (0);
	if (data->tokens[3] != NULL)
	{
		errno = E2BIG;
		perror(data->command_name);
		return (5);
	}

	env_set_key(data->tokens[1], data->tokens[2], data);

	return (0);
}

/**
 * builtin_unset_env - removes an environment variable
 * @data: program data structure
 * Return: 0 if successful, or an error code if specified in the arguments
 */
int builtin_unset_env(program_data *data)
{
	/* validate arguments */
	if (data->tokens[1] == NULL)
		return (0);
	if (data->tokens[2] != NULL)
	{
		errno = E2BIG;
		perror(data->command_name);
		return (5);
	}
	env_remove_key(data->tokens[1], data);

	return (0);
}
