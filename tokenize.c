#include "shellLib.h"
/**
 * tokenize - this function separate the string using a designed delimiter
 * @data: a pointer to the program's data
 * Return: an array of the different parts of the string
 */
void tokenize(program_data *data)
{
    char *delim = " \t";
    int i, j, counter = 2, len;

    len = str_length(data->input_line);
    if (len)
    {
        if (data->input_line[len -1] == '\n')
            data->input_line[len - 1] = '\0';
    }

    for (i = 0; data->input_line[i]; i++)
    {
        for (j = 0; delim[j]; j++)
        {
            if (data->input_line[i] == delim[j])
                counter++;
        }
    }

    data->tokens = malloc(counter * sizeof(char *));
    if (data->tokens == NULL)
    {
        perror(data->program_name);
        exit(errno);
    }
    i = 0;
    data->tokens[i] = str_duplicate(_strtok(data->input_line, delim));
    data->command_name = str_duplicate(data->tokens[0]);
    while (data->tokens[i++])
    {
        data->tokens[i] = str_duplicate(_strtok(NULL, delim));
    }
}
