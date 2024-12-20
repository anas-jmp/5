#include "../../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_REDIRECT_SIZE 4 // Initial size for redirection arrays
#define INITIAL_ARGS_SIZE 16


char *extract_variable_name(const char **cursor)
{
    const char *start = *cursor;
    while (isalnum(**cursor) || **cursor == '_' || **cursor == '?')
        (*cursor)++;
    size_t len = *cursor - start;

    char *var_name = malloc(len + 1);
    if (var_name)
    {
        strncpy(var_name, start, len);
        var_name[len] = '\0';
    }
    return var_name;
}

void handle_variable_expansion(const char **cursor, char **res_cursor, char **env)
{
    char *var_name = extract_variable_name(cursor);
    if (var_name)
    {
        char *expanded_value = expand_variable(var_name, env);
        free(var_name);

        if (expanded_value)
        {
            strcpy(*res_cursor, expanded_value);
            *res_cursor += strlen(expanded_value);
            free(expanded_value);
        }
    }
}
 void copy_regular_characters(const char **cursor, char **res_cursor)
{
    **res_cursor = **cursor;
    (*res_cursor)++;
    (*cursor)++;
}

char *allocate_and_duplicate_result(const char *result, char *expanded_input)
{
    char *final_result = strdup(result);
    free((char *)result);
    free(expanded_input);
    return final_result;
}

char *handle_quoted_string_with_expansion(char *input, char **env)
{
    char *expanded_input;
    char *result;
    const char *cursor;
    char *res_cursor;

    expanded_input = strdup(input);
    if (!expanded_input)
        return NULL;

     result = malloc(4096*4);
    if (!result)
    {
        free(expanded_input);
        free(result);
        return NULL;
    }
    cursor = expanded_input;
    res_cursor = result;
    process_cursor(&cursor, &res_cursor, env);
    *res_cursor = '\0';
  
    return allocate_and_duplicate_result(result, expanded_input);
}