#include "shellLib.h"

/**
 * env_get_key - retrieves the value of an environment variable by its key
 * @key: the key of the environment variable to retrieve
 * @data: a struct containing the program's data
 * Return: a pointer to the value of the variable, or NULL if it doesn't exist
 */
char *env_get_key(char *key, program_data *data)
{
	int i, key_length = 0;

	/* Validate the arguments */
	if (key == NULL || data->env == NULL)
		return (NULL);

	/* Get the length of the requested variable */
	key_length = str_length(key);

	for (i = 0; data->env[i]; i++)
	{
		/* Iterate through the environment variables and check for a match */
		if (str_compare(key, data->env[i], key_length) &&
			data->env[i][key_length] == '=')
		{
			/* Return the value of the variable when a match is found */
			return (data->env[i] + key_length + 1);
		}
	}
	/* Return NULL if the variable doesn't exist */
	return (NULL);
}

/**
 * env_set_key - overwrites the value of an environment variable or creates it if it doesn't exist
 * @key: the key of the environment variable to set
 * @value: the new value of the variable
 * @data: a struct containing the program's data
 * Return: 1 if the parameters are NULL, 2 if there is an error, or 0 if successful
 */

int env_set_key(char *key, char *value, program_data *data)
{
	int i, key_length = 0, is_new_key = 1;

	/* Validate the arguments */
	if (key == NULL || value == NULL || data->env == NULL)
		return (1);

	/* Get the length of the requested variable */
	key_length = str_length(key);

	for (i = 0; data->env[i]; i++)
	{
		/* Iterate through the environment variables and check for a match */
		if (str_compare(key, data->env[i], key_length) &&
			data->env[i][key_length] == '=')
		{
			/* If the variable already exists, free the memory and break */
			is_new_key = 0;
			free(data->env[i]);
			break;
		}
	}
	/* Create a string in the format "key=value" */
	data->env[i] = str_concat(str_duplicate(key), "=");
	data->env[i] = str_concat(data->env[i], value);

	if (is_new_key)
	{
		/* If the variable is new, create it at the end of the list and put NULL in the next position */
		data->env[i + 1] = NULL;
	}
	return (0);
}

/**
 * env_remove_key - removes an environment variable by its key
 * @key: the key of the environment variable to remove
 * @data: a struct containing the program's data
 * Return: 1 if the variable was removed, 0 if it doesn't exist
 */
int env_remove_key(char *key, program_data *data)
{
	int i, key_length = 0;

	/* validate the arguments */
	if (key == NULL || data->env == NULL)
		return (0);

	/* obtains the leng of the variable requested */
	key_length = str_length(key);

	for (i = 0; data->env[i]; i++)
	{ /* iterates through the environ and checks for coincidences */
		if (str_compare(key, data->env[i], key_length) &&
			data->env[i][key_length] == '=')
		{ /* if key already exists, remove them */
			free(data->env[i]);

			/* move the others keys one position down */
			i++;
			for (; data->env[i]; i++)
			{
				data->env[i - 1] = data->env[i];
			}
			/* put the NULL value at the new end of the list */
			data->env[i - 1] = NULL;
			return (1);
		}
	}
	return (0);
}

/**
 * print_environ - prints the current environ
 * @data: struct for the program's data
 * Return: nothing
 */
void print_environ(program_data *data)
{
	int j;

	for (j = 0; data->env[j]; j++)
	{
		_print(data->env[j]);
		_print("\n");
	}
}