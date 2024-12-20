#include "../../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_REDIRECT_SIZE 4 // Initial size for redirection arrays
#define INITIAL_ARGS_SIZE 16


// #define INITIAL_ARGS_SIZE 1024  // Adjust this later if necessary
 

t_command *init_command()
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;

    cmd->command = NULL;
    cmd->args = malloc(INITIAL_ARGS_SIZE * sizeof(char *));
    cmd->input_redirect = malloc(INITIAL_REDIRECT_SIZE * sizeof(char *));
    cmd->output_redirect = malloc(INITIAL_REDIRECT_SIZE * sizeof(char *));
    cmd->here_docs = malloc(INITIAL_REDIRECT_SIZE * sizeof(char *));
    cmd->append_modes = malloc(INITIAL_REDIRECT_SIZE * sizeof(int));
    cmd->herdoc_last = 0;
    cmd->here_doc_last_fd = 0;
    cmd->next = NULL;

    if (!cmd->here_docs || !cmd->args || !cmd->input_redirect || !cmd->output_redirect || !cmd->append_modes) 
    {
        free_command(cmd); // Free allocated memory in case of failure
        return NULL;
    }

    return cmd;
}


void process_cursor(const char **cursor, char **res_cursor, char **env)
{
    while (**cursor)
    {
        if (**cursor == '$')
        {
            if ((*cursor)[1] == ' ' || (*cursor)[1] == '\0')
            {
                copy_regular_characters(cursor, res_cursor); // Copy the '$' as is
            }
            else
            {
                (*cursor)++; // Skip the '$'
                handle_variable_expansion(cursor, res_cursor, env);
            }
        }
        else
        {
            copy_regular_characters(cursor, res_cursor);
        }
    }
}