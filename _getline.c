
#include "shellLib.h"

/**
 * _getline - read one line from the prompt.
 * @data: struct for the program's data.
 *
 * Return: reading counting bytes.
 */
int _getline(program_data *data)
{
    char buff[BUFFER_SIZE] = {'\0'};
    static char *arr_cmds[10] = {NULL};
    static char arr_ops[10] = {'\0'};
    ssize_t bytes_read, i = 0;

    /* check if doesnot exist more commands in the array */
    /* and checks the logical operators */
    if (!arr_cmds[0] || (arr_ops[0] == '&' && errno != 0) ||
        (arr_ops[0] == '|' && errno == 0))
    {
        /*free the memory allocated in the array if it exists */
        for (i = 0; arr_cmds[i]; i++)
        {
            free(arr_cmds[i]);
            arr_cmds[i] = NULL;
        }

        /* read from the file descriptor int to buff */
        bytes_read = read(data->file_descriptor, &buff, BUFFER_SIZE - 1);
        if (bytes_read == 0)
            return (-1);

        /* split lines for \n or ; */
        i = 0;
        do
        {
            arr_cmds[i] = str_duplicate(_strtok(i ? NULL : buff, "\n;"));
            /*checks and split for && and || operators*/
            i = check_logic_ops(arr_cmds, i, arr_ops);
        } while (arr_cmds[i++]);
    }

    /*obtains the next command (command 0) and remove it for the array*/
    data->input_line = arr_cmds[0];
    for (i = 0; arr_cmds[i]; i++)
    {
        arr_cmds[i] = arr_cmds[i + 1];
        arr_ops[i] = arr_ops[i + 1];
    }

    return (str_length(data->input_line));
}

/**
 * check_logic_ops - checks and split for && and || operators
 * @arr_cmds: array of the commands.
 * @i: index in the arr_cmds to be checked
 * @arr_ops: array of the logical operators for each previous command
 *
 * Return: index of the last command in the arr_cmds.
 */
int check_logic_ops(char *arr_cmds[], int i, char arr_ops[])
{
    char *temp = NULL;
    int j;

    /* checks for the & char in the command line*/
    for (j = 0; arr_cmds[i] != NULL && arr_cmds[i][j]; j++)
    {
        if (arr_cmds[i][j] == '&' && arr_cmds[i][j + 1] == '&')
        {
            /* split the line when chars && was found */
            temp = arr_cmds[i];
            arr_cmds[i][j] = '\0';
            arr_cmds[i] = str_duplicate(arr_cmds[i]);
            arr_cmds[i + 1] = str_duplicate(temp + j + 2);
            i++;
            arr_ops[i] = '&';
            free(temp);
            j = 0;
        }
        if (arr_cmds[i][j] == '|' && arr_cmds[i][j + 1] == '|')
        {
            /* split the line when chars || was found */
            temp = arr_cmds[i];
            arr_cmds[i][j] = '\0';
            arr_cmds[i] = str_duplicate(arr_cmds[i]);
            arr_cmds[i + 1] = str_duplicate(temp + j + 2);
            i++;
            arr_ops[i] = '|';
            free(temp);
            j = 0;
        }
    }
    return (i);
}