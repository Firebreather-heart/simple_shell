#include "shellLib.h"

/**
 * print_alias - print a specific alias or all aliases
 * @data: struct containing the program data
 * @alias: name of the specific alias to be printed
 * Return: 0 on success, or a non-zero value if there is an error
 */
int print_alias(program_data *data, char *alias)
{
	int i, j, alias_length;
	char buffer[250] = {'\0'};

	if (data->alias_list)
	{
		alias_length = str_length(alias);
		for (i = 0; data->alias_list[i]; i++)
		{
			/*if alias is NULL or if the current alias matches the requested alias*/
			if (!alias || (str_compare(data->alias_list[i], alias, alias_length) && data->alias_list[i][alias_length] == '='))
			{
				/*copy the alias name to the buffer*/
				for (j = 0; data->alias_list[i][j]; j++)
				{
					buffer[j] = data->alias_list[i][j];
					if (data->alias_list[i][j] == '=')
						break;
				}
				buffer[j + 1] = '\0';

				/* add quotes around the alias value and print the alias */
				buffer_add(buffer, "'");
				buffer_add(buffer, data->alias_list[i] + j + 1);
				buffer_add(buffer, "'\n");
				_print(buffer);
			}
		}
	}

	return (0);
}

/**
 * get_alias - retrieve the value of a specific alias
 * @data: struct containing program data
 * @name: name of the alias to retrieve
 * Return: a pointer to the alias value, or NULL if the alias does not exist
 */
char *get_alias(program_data *data, char *name)
{
	int i, alias_length;

	/* validate arguments */
	if (name == NULL || data->alias_list == NULL)
		return (NULL);

	alias_length = str_length(name);

	for (i = 0; data->alias_list[i]; i++)
	{
		/* if the current alias matches the requested alias */
		if (str_compare(name, data->alias_list[i], alias_length) &&
			data->alias_list[i][alias_length] == '=')
		{
			/* return the alias value */
			return (data->alias_list[i] + alias_length + 1);
		}
	}

	/* the alias does not exist */
	return (NULL);
}

/**
 * set_alias - add or update an alias
 * @alias_string: the alias to add or update, in the form "name='value'"
 * @data: struct containing program data
 * Return: 0 on success, or a non-zero value if there is an error
 */
int set_alias(char *alias_string, program_data *data)
{
	int i, j;
	char buffer[250] = {'0'}, *temp = NULL;

	/* validate the arguments */
	if (alias_string == NULL || data->alias_list == NULL)
		return (1);
	/* Iterates alias to find = char */
	for (i = 0; alias_string[i]; i++)
		if (alias_string[i] != '=')
			buffer[i] = alias_string[i];
		else
		{ /* search if the value of the alias is another alias */
			temp = get_alias(data, alias_string + i + 1);
			break;
		}

	/* Iterates through the alias list and check for coincidence of the varname */
	for (j = 0; data->alias_list[j]; j++)
		if (str_compare(buffer, data->alias_list[j], i) &&
			data->alias_list[j][i] == '=')
		{ /* if the alias alredy exist */
			free(data->alias_list[j]);
			break;
		}

	/* add the alias */
	if (temp)
	{ /* if the alias already exist */
		buffer_add(buffer, "=");
		buffer_add(buffer, temp);
		data->alias_list[j] = str_duplicate(buffer);
	}
	else /* if the alias does not exist */
		data->alias_list[j] = str_duplicate(alias_string);
	return (0);
}
