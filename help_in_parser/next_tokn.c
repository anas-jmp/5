#include "../../include/minishell.h"

/***********************/
char *extract_next_token(const char **str, char **env) 
{
    int status=0;
    while (isspace(**str))
        (*str)++;
    if (**str == '\0') // End of string
        return NULL;
    const char *start = *str;
if (**str == '"' || **str == '\'')
    {
        char quote = **str;
        (*str)++;
        const char *quoted_start = *str;
        while (**str && **str != quote)
            (*str)++;
        size_t len = *str - quoted_start;
        char *token_value = malloc(len + 1);
        strncpy(token_value, quoted_start, len);
        token_value[len] = '\0';
        if (**str == quote)
            (*str)++;
        if (quote == '\'') 
             return token_value;
        else
         {
             char *expanded_token = handle_quoted_string_with_expansion(token_value, env);
            free(token_value);
            return expanded_token;
         }
    }
 if (**str == '$')
   {
    (*str)++;
    const char *var_start = *str;

    while (isalnum(**str) || **str == '_' || **str == '?')
      (*str)++;

    size_t len = *str - var_start;
    char var_name[len + 1];
    strncpy(var_name, var_start, len);
    var_name[len] = '\0';
    char *expanded_value = expand_variable(var_name, env);
    if (expanded_value) {
      return expanded_value;
    } else {
      return strdup("");
    }
  }
    if (**str == '>' || **str == '<' || **str == '|') 
    {
        char symbol = **str;
        (*str)++;
        if (symbol == '>' && **str == '>')
         (*str)++;
          return strdup(">>");
        if (symbol == '<' && **str == '<')
           (*str)++; 
           return strdup("<<"); 
        char temp[2] = {symbol, '\0'};
        return strdup(temp);
    }
    while (**str && !isspace(**str) && **str != '>' && **str != '<' &&
           **str != '|' && **str != '\'' && **str != '"')
        (*str)++;
    size_t len = *str - start;
    char *token = malloc(len + 1);
    strncpy(token, start, len);
    token[len] = '\0';
    return token;
}

/***************************************/

token_type determine_token_type(const char *token, const char **str, char **env)
{
    if (strcmp(token, "echo") == 0)
        return WORD;
    if (strcmp(token, "export") == 0)
    {
        handle_export_command(*str);
        return WORD;
    }
    if (strcmp(token, "unset") == 0)
    {
        handle_unset(*str);
        return WORD;
    }
    if (strcmp(token, ">") == 0)
        return OUTPUT_REDIRECTION;
    if (strcmp(token, ">>") == 0)
        return OUTPUT_REDIRECTION_APPEND_MODE;
    if (strcmp(token, "<") == 0)
        return INPUT_REDIRECTION;
    if (strcmp(token, "<<") == 0)
        return HERE_DOC;
    if (strcmp(token, "|") == 0)
        return PIPES;
    if (token[0] == '$') 
    {
        char *expanded_value = expand_variable(token + 1, env, NULL);
        if (expanded_value) {
            free((char *)token); // Free the original token
            token = strdup(expanded_value); // Replace with expanded value
            free(expanded_value);
            return EXPANDABLE;
        }
        return EXPANDABLE; // No expansion found
    }
    return WORD; // Default type
}


char *next_token(const char **str, char **env, token_type *type) 
{
    char *token = extract_next_token(str,env); // Extract token
    if (!token)
        return NULL;

    *type = determine_token_type(token, str, env); // Determine type
    return token; // Return token to caller
}


