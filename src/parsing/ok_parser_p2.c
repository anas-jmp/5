#include "../../include/minishell.h"
#define INITIAL_REDIRECT_SIZE 4 // Initial size for redirection arrays
#define INITIAL_ARGS_SIZE 16
#include <stdbool.h> // for bool, true, false
 

t_command *initialize_command() 
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

    if (!cmd->args || !cmd->input_redirect || !cmd->output_redirect || 
        !cmd->here_docs || !cmd->append_modes) {
        free_command(cmd);  // Ensure to clean up properly
        return NULL;
    }

    return cmd;
}

void finalize_command(t_command *cmd, int arg_count, int input_count, int output_count, int heredoc_count) 
{
    if (!cmd)
        return;

    cmd->args[arg_count] = NULL;
    cmd->input_redirect[input_count] = NULL;
    cmd->output_redirect[output_count] = NULL;
    cmd->here_docs[heredoc_count] = NULL;
}

int handle_command(t_command *cmd, token **tokens, int *arg_count)
{
    if (!cmd->command && (*tokens)->token_type == WORD) {
        cmd->command = strdup((*tokens)->value);
        cmd->args[(*arg_count)++] = strdup(cmd->command);
        return 1;
    }
    return 0;
}

// int process_tokens(token **tokens, t_command *cmd, int *arg_count, int *input_count, int *output_count, int *heredoc_count)
//  {
//      int max_args;
//     int max_inputs;
//     int max_outputs;
//     int max_heredocs;
     
//       max_inputs = INITIAL_REDIRECT_SIZE;
//       max_outputs = INITIAL_REDIRECT_SIZE;
//       max_heredocs = INITIAL_REDIRECT_SIZE;
//       max_args = INITIAL_ARGS_SIZE;
//     while (*tokens && (*tokens)->token_type != PIPES) {
//         if (set_command_and_args(tokens, cmd, arg_count) || handle_single_quoted_string(tokens, cmd, arg_count))
//             continue;
//         if ((*tokens)->token_type == WORD || (*tokens)->token_type == DOUBLE_QUOTED_STRING || (*tokens)->token_type == SINGLE_QUOTED_STRING)
//             cmd->args[(*arg_count)++] = strdup((*tokens)->value);
//         if (!handle_input_redirection(cmd, tokens, input_count, &max_inputs) || 
//             !handle_output_redirection(cmd, tokens, output_count, &max_outputs) || 
//             !handle_heredoc_redirection(cmd, tokens, heredoc_count, &max_heredocs))
//             return 0; // Failure
//         *tokens = (*tokens)->next;
//     }
//     return 1; // Success
// }


    // int max_args;
    // int max_inputs;
    // int max_outputs;
    // int max_heredocs;
     
    //   max_inputs = INITIAL_REDIRECT_SIZE;
    //   max_outputs = INITIAL_REDIRECT_SIZE;
    //   max_heredocs = INITIAL_REDIRECT_SIZE;
    //   max_args = INITIAL_ARGS_SIZE;

int process_tokens(token **tokens, t_command *cmd, t_parser *parser)
 {   
     t_count val;

      val.max_inputs = INITIAL_REDIRECT_SIZE;
      val.max_outputs = INITIAL_REDIRECT_SIZE;
      val.max_heredocs = INITIAL_REDIRECT_SIZE;
      val.max_args = INITIAL_ARGS_SIZE;
    while (*tokens && (*tokens)->token_type != PIPES) {
        if (set_command_and_args(tokens, cmd, &parser->arg_count) ||
            handle_single_quoted_string(tokens, cmd, &parser->arg_count))
            continue;
        if ((*tokens)->token_type == WORD || (*tokens)->token_type == DOUBLE_QUOTED_STRING || (*tokens)->token_type == SINGLE_QUOTED_STRING)
        {
            cmd->args[parser->arg_count++] = strdup((*tokens)->value);
        }
        if (!handle_input_redirection(cmd, tokens, &parser->input_count, &val.max_inputs) ||
            !handle_output_redirection(cmd, tokens, &parser->output_count, &val.max_outputs) ||
            !handle_heredoc_redirection(cmd, tokens, &parser->heredoc_count, &val.max_heredocs)) {
            return 0; // Failure
        }
        *tokens = (*tokens)->next;
    }
    return 1; // Success
}


void add_command_to_listt(t_command **head, t_command **current, token **tokens, t_command *cmd)
{
    if (!*head) {
        *head = cmd;
    } else {
        (*current)->next = cmd;
    }
    *current = cmd;

    // Skip the pipe token if present
    if (*tokens && (*tokens)->token_type == PIPES) {
        *tokens = (*tokens)->next;
    }
}

t_command *parse_command(token *tokens)
{
        t_command *head;
        t_command *current;
        t_parser parser;

      head = NULL;
     current = NULL;
      if (!tokens)
        return NULL;
    while (tokens) 
    {
        t_command *cmd = initialize_command();
        if (!cmd)
            return NULL;
        parser.arg_count = 0;
        parser.input_count = 0;
        parser.output_count = 0;
        parser.heredoc_count = 0;
        
        if (!process_tokens(&tokens, cmd, &parser))
            return NULL;
        finalize_command(cmd, parser.arg_count,parser.input_count,parser.output_count,parser.heredoc_count);
         add_command_to_listt(&head, &current, &tokens, cmd);
    }
    return head;
}
