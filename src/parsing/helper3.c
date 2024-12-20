#include "../../include/minishell.h"
#define INITIAL_REDIRECT_SIZE 4 // Initial size for redirection arrays
#define INITIAL_ARGS_SIZE 16
#include <stdbool.h> // for bool, true, false


bool handle_single_quoted_string(token **tokens, t_command *cmd, int *arg_count) {
    if (!tokens || !*tokens || (*tokens)->token_type != SINGLE_QUOTED_STRING)
        return false; // Not a single-quoted string or invalid input

    // Add the single-quoted string as-is to the arguments
    cmd->args[(*arg_count)++] = strdup((*tokens)->value);
    if (!cmd->args[(*arg_count) - 1]) // Handle memory allocation failure
        return false;

    *tokens = (*tokens)->next; // Move to the next token
    return true; // Successfully handled the single-quoted string
}
 
bool set_command_and_args(token **tokens, t_command *cmd, int *arg_count)
 {
    if (!cmd->command && 
        ((*tokens)->token_type == WORD || 
         (*tokens)->token_type == DOUBLE_QUOTED_STRING || 
         (*tokens)->token_type == SINGLE_QUOTED_STRING)) 
    {
        cmd->command = strdup((*tokens)->value);
        if (!cmd->command) // Handle memory allocation failure
            return false;

        cmd->args[(*arg_count)++] = strdup(cmd->command);
        if (!cmd->args[(*arg_count) - 1]) { // Handle memory allocation failure
            free(cmd->command);
            cmd->command = NULL;
            return false;
        }

        *tokens = (*tokens)->next; // Move to the next token
        return true; // Successfully processed command and args
    }

    return false; // Logic was not applicable (cmd->command already set or invalid token)
}


int handle_input_redirection(t_command *cmd, token **tokens, int *input_count, int *max_inputs)
 {
    if ((*tokens)->token_type == INPUT_REDIRECTION) {
        *tokens = (*tokens)->next;
        if (*tokens && ((*tokens)->token_type == WORD || (*tokens)->token_type == DOUBLE_QUOTED_STRING || (*tokens)->token_type == SINGLE_QUOTED_STRING)) {
            if (*input_count >= *max_inputs - 1) {
                *max_inputs *= 2;
                cmd->input_redirect = resize_array(cmd->input_redirect, *input_count, *max_inputs);
                if (!cmd->input_redirect) {
                    return 0;  // Memory allocation failure
                }
            }
            cmd->input_redirect[(*input_count)++] = strdup((*tokens)->value);
            return 1;
        } else {
            printf("\nError: No file specified for input redirection\n");
            return 0;
        }
    }
    return 1;  // No redirection
}

int handle_output_redirection(t_command *cmd, token **tokens, int *output_count, int *max_outputs) {
    if ((*tokens)->token_type == OUTPUT_REDIRECTION || (*tokens)->token_type == OUTPUT_REDIRECTION_APPEND_MODE) {
        int append = (*tokens)->token_type == OUTPUT_REDIRECTION_APPEND_MODE;
        *tokens = (*tokens)->next;
        if (*tokens && ((*tokens)->token_type == WORD || (*tokens)->token_type == DOUBLE_QUOTED_STRING || (*tokens)->token_type == SINGLE_QUOTED_STRING)) {
            if (*output_count >= *max_outputs - 1) {
                *max_outputs *= 2;
                cmd->output_redirect = resize_array(cmd->output_redirect, *output_count, *max_outputs);
                cmd->append_modes = resize_int_array(cmd->append_modes, *output_count, *max_outputs);
                if (!cmd->output_redirect || !cmd->append_modes) {
                    return 0;  // Memory allocation failure
                }
            }
            cmd->output_redirect[*output_count] = strdup((*tokens)->value);
            cmd->append_modes[*output_count] = append;
            (*output_count)++;
            return 1;
        } else {
            return 0;  // Missing file for redirection
        }
    }
    return 1;  // No redirection
}

int handle_heredoc_redirection(t_command *cmd, token **tokens, int *heredoc_count, int *max_heredocs)
 {
    if ((*tokens)->token_type == HERE_DOC) {
        *tokens = (*tokens)->next;
        if (*tokens && ((*tokens)->token_type == WORD || (*tokens)->token_type == DOUBLE_QUOTED_STRING || (*tokens)->token_type == SINGLE_QUOTED_STRING)) {
            if (*heredoc_count >= *max_heredocs - 1) {
                *max_heredocs *= 2;
                cmd->here_docs = resize_array(cmd->here_docs, *heredoc_count, *max_heredocs);
                if (!cmd->here_docs) {
                    return 0;  // Memory allocation failure
                }
            }
            cmd->here_docs[(*heredoc_count)++] = strdup((*tokens)->value);
            return 1;
        } else {
            printf("\nError: No file specified for heredoc redirection\n");
            return 0;
        }
    }
    return 1;  // No here-doc
}
 