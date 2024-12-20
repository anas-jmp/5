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
    cmd->arg_count = 0;       // Initialize counters
    cmd->input_count = 0;
    cmd->output_count = 0;
    cmd->heredoc_count = 0;
    cmd->next = NULL;

    if (!cmd->args || !cmd->input_redirect || !cmd->output_redirect || 
        !cmd->here_docs || !cmd->append_modes) {
        free_command(cmd);
        return NULL;
    }
    return cmd;
}


void handle_arguments(token **tokens, t_command *cmd, char **env) {
    int *status = 0;

    while (*tokens && ((*tokens)->token_type == WORD ||
                       (*tokens)->token_type == DOUBLE_QUOTED_STRING ||
                       (*tokens)->token_type == SINGLE_QUOTED_STRING)) {
        char *arg_value = (*tokens)->value;

        if (arg_value[0] == '$') {
            char *var_name = arg_value + 1;
            char *expanded_value = expand_variable(var_name, env, status);
            if (expanded_value) {
                arg_value = expanded_value;
            }
        }

        if (cmd->arg_count >= INITIAL_ARGS_SIZE - 1) {
            cmd->args = resize_array(cmd->args, cmd->arg_count, cmd->arg_count * 2);
            if (!cmd->args) {
                free_command(cmd);
                return;
            }
        }

        cmd->args[cmd->arg_count++] = strdup(arg_value);
        *tokens = (*tokens)->next;
    }
    cmd->args[cmd->arg_count] = NULL;
}




void handle_input_redirection(token **tokens, t_command *cmd) {
    if (!*tokens || (*tokens)->token_type != INPUT_REDIRECTION)
        return;

    *tokens = (*tokens)->next;
    if (*tokens && ((*tokens)->token_type == WORD ||
                    (*tokens)->token_type == SINGLE_QUOTED_STRING ||
                    (*tokens)->token_type == DOUBLE_QUOTED_STRING)) {
        if (cmd->input_count >= INITIAL_REDIRECT_SIZE - 1) {
            cmd->input_redirect = resize_array(cmd->input_redirect, cmd->input_count, cmd->input_count * 2);
            if (!cmd->input_redirect) {
                free_command(cmd);
                return;
            }
        }

        cmd->input_redirect[cmd->input_count++] = strdup((*tokens)->value);
        *tokens = (*tokens)->next;
    }
}


void handle_heredoc(token **tokens, t_command *cmd)
{
    if (!*tokens || (*tokens)->token_type != HERE_DOC)
        return;

    *tokens = (*tokens)->next;
    if (*tokens && ((*tokens)->token_type == WORD ||
                    (*tokens)->token_type == SINGLE_QUOTED_STRING ||
                    (*tokens)->token_type == DOUBLE_QUOTED_STRING)) {
        if (cmd->heredoc_count >= INITIAL_REDIRECT_SIZE - 1) {
            cmd->here_docs = resize_array(cmd->here_docs, cmd->heredoc_count, cmd->heredoc_count * 2);
            if (!cmd->here_docs) {
                free_command(cmd);
                return;
            }
        }

        cmd->here_docs[cmd->heredoc_count++] = strdup((*tokens)->value);
        cmd->herdoc_last = 1;
        *tokens = (*tokens)->next;
    }
}



t_command *parse_command(token *tokens, char **env)
 {
    if (!tokens)
        return NULL;

    t_command *head = NULL;
    t_command *current = NULL;

    while (tokens) {
        t_command *cmd = init_command(); // Initialize a new command structure
        if (!cmd)
            return NULL;

        // Handle each part of the command
        handle_command_name(&tokens, cmd);
        handle_arguments(&tokens, cmd, env);
        handle_input_redirection(&tokens, cmd);
        handle_output_redirection(&tokens, cmd);
        handle_heredoc(&tokens, cmd);

        // Append the command to the linked list
        append_command(&head, &current, cmd);

        // Move to the next token if it's a pipe
        if (tokens && tokens->token_type == PIPES)
            tokens = tokens->next;
    }

    return head; // Return the head of the linked list
}


// /***********************/
// t_command *init_command() {
//     t_command *cmd = malloc(sizeof(t_command));
//     if (!cmd)
//         return NULL;

//     cmd->command = NULL;
//     cmd->args = malloc(INITIAL_ARGS_SIZE * sizeof(char *));
//     cmd->input_redirect = malloc(INITIAL_REDIRECT_SIZE * sizeof(char *));
//     cmd->output_redirect = malloc(INITIAL_REDIRECT_SIZE * sizeof(char *));
//     cmd->here_docs = malloc(INITIAL_REDIRECT_SIZE * sizeof(char *));
//     cmd->append_modes = malloc(INITIAL_REDIRECT_SIZE * sizeof(int));
//     cmd->herdoc_last = 0;
//     cmd->here_doc_last_fd = 0;
//     cmd->next = NULL;

//     if (!cmd->args || !cmd->input_redirect || !cmd->output_redirect ||
//         !cmd->here_docs || !cmd->append_modes) {
//         free_command(cmd); // Free allocated memory if initialization fails
//         return NULL;
//     }

//     // Initialize arrays to NULL
//     for (int i = 0; i < INITIAL_ARGS_SIZE; i++) cmd->args[i] = NULL;
//     for (int i = 0; i < INITIAL_REDIRECT_SIZE; i++) {
//         cmd->input_redirect[i] = NULL;
//         cmd->output_redirect[i] = NULL;
//         cmd->here_docs[i] = NULL;
//     }

//     return cmd;
// }

// void handle_command_name(token **tokens, t_command *cmd) 
// {
//     if (!cmd->command && ((*tokens)->token_type == WORD ||
//                           (*tokens)->token_type == DOUBLE_QUOTED_STRING ||
//                           (*tokens)->token_type == SINGLE_QUOTED_STRING)) {
//         cmd->command = strdup((*tokens)->value);
//         cmd->args[0] = strdup(cmd->command); // Add the command as the first argument
//         *tokens = (*tokens)->next;
//     }
// }
// void handle_arguments(token **tokens, t_command *cmd, char **env) 
// {
//     int *status = 0;
//     int arg_count = 1; // Start after the command

//     while (*tokens && ((*tokens)->token_type == WORD ||
//                        (*tokens)->token_type == DOUBLE_QUOTED_STRING ||
//                        (*tokens)->token_type == SINGLE_QUOTED_STRING)) {
//         char *arg_value = (*tokens)->value;

//         if (arg_value[0] == '$') {
//             char *var_name = arg_value + 1;
//             char *expanded_value = expand_variable(var_name, env, status);
//             if (expanded_value)
//                 arg_value = expanded_value;
//         }

//         if (arg_count >= INITIAL_ARGS_SIZE - 1) {
//             cmd->args = resize_array(cmd->args, arg_count, arg_count * 2);
//             if (!cmd->args) {
//                 free_command(cmd);
//                 return;
//             }
//         }

//         cmd->args[arg_count++] = strdup(arg_value);
//         *tokens = (*tokens)->next;
//     }
//     cmd->args[arg_count] = NULL; // Null-terminate the arguments array
// }
// void handle_input_redirection(token **tokens, t_command *cmd) {
//     static int input_count = 0;

//     if (!*tokens || (*tokens)->token_type != INPUT_REDIRECTION)
//         return;

//     *tokens = (*tokens)->next; // Skip '<'

//     if (*tokens && ((*tokens)->token_type == WORD ||
//                     (*tokens)->token_type == SINGLE_QUOTED_STRING ||
//                     (*tokens)->token_type == DOUBLE_QUOTED_STRING)) {
//         if (input_count >= INITIAL_REDIRECT_SIZE - 1) {
//             cmd->input_redirect = resize_array(cmd->input_redirect, input_count, input_count * 2);
//             if (!cmd->input_redirect)
//                 return;
//         }
//         cmd->input_redirect[input_count++] = strdup((*tokens)->value);
//         *tokens = (*tokens)->next;
//     }
//     cmd->input_redirect[input_count] = NULL; // Null-terminate
// }
// void handle_output_redirection(token **tokens, t_command *cmd) {
//     static int output_count = 0;

//     int append = (*tokens)->token_type == OUTPUT_REDIRECTION_APPEND_MODE;
//     *tokens = (*tokens)->next; // Skip '>' or '>>'

//     if (*tokens && ((*tokens)->token_type == WORD ||
//                     (*tokens)->token_type == SINGLE_QUOTED_STRING ||
//                     (*tokens)->token_type == DOUBLE_QUOTED_STRING)) {
//         if (output_count >= INITIAL_REDIRECT_SIZE - 1) {
//             cmd->output_redirect = resize_array(cmd->output_redirect, output_count, output_count * 2);
//             cmd->append_modes = resize_int_array(cmd->append_modes, output_count, output_count * 2);
//             if (!cmd->output_redirect || !cmd->append_modes)
//                 return;
//         }
//         cmd->output_redirect[output_count] = strdup((*tokens)->value);
//         cmd->append_modes[output_count++] = append;
//         *tokens = (*tokens)->next;
//     }
//     cmd->output_redirect[output_count] = NULL; // Null-terminate
// }
// void handle_heredoc(token **tokens, t_command *cmd) 
// {
//     static int heredoc_count = 0;

//     if (!*tokens || (*tokens)->token_type != HERE_DOC)
//         return;

//     *tokens = (*tokens)->next; // Skip '<<'

//     if (*tokens && ((*tokens)->token_type == WORD ||
//                     (*tokens)->token_type == SINGLE_QUOTED_STRING ||
//                     (*tokens)->token_type == DOUBLE_QUOTED_STRING)) {
//         if (heredoc_count >= INITIAL_REDIRECT_SIZE - 1) {
//             cmd->here_docs = resize_array(cmd->here_docs, heredoc_count, heredoc_count * 2);
//             if (!cmd->here_docs)
//                 return;
//         }
//         cmd->here_docs[heredoc_count++] = strdup((*tokens)->value);
//         *tokens = (*tokens)->next;
//     }
//     cmd->here_docs[heredoc_count] = NULL; // Null-terminate
// }
// void append_command(t_command **head, t_command **current, t_command *cmd)
// {
//     if (!*head) {
//         *head = cmd;
//     } else {
//         (*current)->next = cmd;
//     }
//     *current = cmd;
// }

// t_command *parse_command(token *tokens, char **env)
// {
//     if (!tokens)
//         return NULL;

//     store_env_variable("?", "0");
//     t_command *head = NULL;
//     t_command *current = NULL;

//     while (tokens)
//     {
//         t_command *cmd = init_command();
//         if (!cmd)
//             return NULL;

//         // Handle parts of the command
//         handle_command_name(&tokens, cmd);
//         handle_arguments(&tokens, cmd, env);
//         handle_input_redirection(&tokens, cmd);
//         handle_output_redirection(&tokens, cmd);
//         handle_heredoc(&tokens, cmd);

//         // Append the parsed command to the linked list
//         append_command(&head, &current, cmd);

//         if (tokens && tokens->token_type == PIPES)
//             tokens = tokens->next;
//     }

//     return head;
// }