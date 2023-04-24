#include "shellLib.h"

/**
 * builtins_list - searches for a match between the given command and a list of
 *                 built-in functions and executes the associated function if a
 *                 match is found
 * @data: struct for the program's data
 *
 * Return: Returns the return value of the executed function if there is a
 *         match, or -1 if there is no match.
 */
int builtins_list(program_data *data)
{
	int iterator;
	builtins options[] = {
		{"exit", builtin_exit},
		{"help", builtin_help},
		{"cd", builtin_cd},
		{"alias", builtin_alias},
		{"env", builtin_env},
		{"setenv", builtin_set_env},
		{"unsetenv", builtin_unset_env},
		{NULL, NULL}
	};

	/* Loop through the list of built-in functions */
	for (iterator = 0; options[iterator].builtin != NULL; iterator++)
	{
		/* If there is a match between the given command and a built-in */
		if (str_compare(options[iterator].builtin, data->command_name, 0))
		{
			/* Execute the associated function and return its return value */
			return (options[iterator].function(data));
		}
	}

	/* If no match was found, return -1 */
	return (-1);
}
