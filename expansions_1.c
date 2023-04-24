#include "shellLib.h"

/**
 * expand_variables - expand variables in the input line
 * @data: a pointer to a struct containing the program's data
 *
 * This function expands special variables in the input line, such as
 * $? and $$, as well as environment variables. It then updates the
 * input line with the expanded version.
 */
void expand_variables(program_data *data)
{
	// Variables to keep track of position in the input line
	int i, j;
	// Buffers for storing the input line and expansions
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	// If the input line is NULL, there's nothing to expand
	if (data->input_line == NULL)
		return;

	// Copy the input line to the line buffer
	buffer_add(line, data->input_line);

	// Loop through the line and look for special variables to expand
	for (i = 0; line[i]; i++)
	{
		// If we encounter a comment symbol (#), ignore the rest of the line
		if (line[i] == '#')
			line[i--] = '\0';
		// If we encounter the special variable $?, replace it with the value of errno
		else if (line[i] == '$' && line[i + 1] == '?')
		{
			line[i] = '\0';
			long_to_string(errno, expansion, 10);
			buffer_add(line, expansion);
			buffer_add(line, data->input_line + i + 2);
		}
		// If we encounter the special variable $$, replace it with the process ID
		else if (line[i] == '$' && line[i + 1] == '$')
		{
			line[i] = '\0';
			long_to_string(getpid(), expansion, 10);
			buffer_add(line, expansion);
			buffer_add(line, data->input_line + i + 2);
		}
		// If we encounter a single $ followed by a space or end-of-line, ignore it
		else if (line[i] == '$' && (line[i + 1] == ' ' || line[i + 1] == '\0'))
			continue;
		// Otherwise, we have an environment variable to expand
		else if (line[i] == '$')
		{
			// Look for the end of the variable name
			for (j = 1; line[i + j] && line[i + j] != ' '; j++)
				expansion[j - 1] = line[i + j];
			// Try to get the value of the environment variable
			temp = env_get_key(expansion, data);
			line[i] = '\0', expansion[0] = '\0';
			buffer_add(expansion, line + i + j);
			// If we found a value, replace the variable with it
			temp ? buffer_add(line, temp) : 1;
			buffer_add(line, expansion);
		}
	}

	// If the input line was changed, update it in the program data
	if (!str_compare(data->input_line, line, 0))
	{
		free(data->input_line);
		data->input_line = str_duplicate(line);
	}
}

/**
 * expand_alias - expand aliases in the input line
 * @data: a pointer to a struct containing the program's data
 *
 * This function expands aliases in the input line, replacing them with
 * their corresponding commands. It then updates the input line with the
 *
