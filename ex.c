/*               look good                     */
#include "../../include/minishell.h"
#include <stdio.h>
#include <string.h>

t_env *env_list = NULL;  // initialize the custom environment list
 

int is_valid_identifier(const char *key)
{
    int i;

    if (!key || !key[0])
        return (0);
     if (!((key[0] >= 'a' && key[0] <= 'z') || 
        (key[0] >= 'A' && key[0] <= 'Z') || key[0] == '_'))
        return (0);
    
    i = 1;
    while (key[i])
    {
        if (!((key[i] >= 'a' && key[i] <= 'z') || 
            (key[i] >= 'A' && key[i] <= 'Z') || 
            (key[i] >= '0' && key[i] <= '9') || 
            key[i] == '_'))
            return (0);
        i++;
    }
    return (1);
}
 
void handle_quoted_value(const char *input, int *i, char **key, char **value, char quote_char)
{
     (*i)++;
    int value_start = *i;
    while (input[*i] && input[*i] != quote_char)
        (*i)++;
    
    *value = malloc(*i - value_start + 1);
    if (!*value) {
        free(*key);
        *key = NULL;
        return;
    }
    strncpy(*value, input + value_start, *i - value_start);
    printf("\nthe string is_2 [%s]\n",*value);
    (*value)[*i - value_start] = '\0';
    
    if (input[*i] == quote_char)
        (*i)++;

}

void handle_unquoted_value(const char *input, int *i, char **key, char **value)
{
     int value_start = *i;
    while (input[*i])
        (*i)++;
    
    *value = malloc(*i - value_start + 1);
    if (!*value)
    {
        free(*key);
        *key = NULL;
        return;
    }
    strncpy(*value, input + value_start, *i - value_start);
     (*value)[*i - value_start] = '\0';
}

void handle_append(char **key, char **value)
{
    t_env *existing = env_list;
    while (existing) {
        if (strcmp(existing->key, *key) == 0) {
            char *new_value;
            int old_len = strlen(existing->value);
            int append_len = strlen(*value);
            
            new_value = malloc(old_len + append_len + 1);
            if (!new_value) {
                free(*key);
                free(*value);
                *key = NULL;
                *value = NULL;
                return;
            }
            strcpy(new_value, existing->value);
            strcat(new_value, *value);
            free(*value);
            *value = new_value;
            break;
        }
        existing = existing->next;
    }
}

void parse_export_value(const char *input, char **key, char **value)
{
     int i = 0;
     int key_start;
     int is_append = 0;
     char quote_char;
    
    *key = NULL;
    *value = NULL;
    if (!input || !*input)
        return;

    while (input[i] && input[i] == ' ')
        i++;
    key_start = i;
    
    if (input[i] == '"' || input[i] == '\'') {
        quote_char = input[i++];
        key_start = i;
        while (input[i] && input[i] != quote_char)
            i++;
        if (input[i] == quote_char)
            i++;
    } else {
        while (input[i] && input[i] != '=' && input[i] != '+' && input[i] != ' ')
            i++;
    }

    *key = malloc(i - key_start + 1);
    if (!*key)
        return;
    strncpy(*key, input + key_start, i - key_start);
    (*key)[i - key_start] = '\0';

    while (input[i] && input[i] == ' ')
        i++;

    if (input[i] == '+' && input[i + 1] == '=') {
        is_append = 1;
        i += 2;
    } else if (input[i] == '=')
        i++;
    else
        return;

    while (input[i] && input[i] == ' ')
        i++;

    if (input[i] == '"' || input[i] == '\'')
        handle_quoted_value(input, &i, key, value, input[i]);
    else
        handle_unquoted_value(input, &i, key, value);

    if (is_append && *value)
        handle_append(key, value);
}

void handle_export_command(char *input)
{
    char *key;
    char *value;
    char *assignment;

    if (!input)
        return;

     assignment = input;
    while (*assignment && *assignment != ' ')
        assignment++;
    while (*assignment && *assignment == ' ')
        assignment++;

    if (!*assignment)
        return;

    parse_export_value(assignment, &key, &value);
    if (key && is_valid_identifier(key))
        store_env_variable(key, value);
    else if (*assignment)
        write(2, "minishell: export: not a valid identifier\n", 40);

    if (key)
        free(key);
    if (value)
        free(value);
}

void store_env_variable(char *key, char *value)
{
    t_env   *current;
    t_env   *new_env;
    char    *new_value;

    if (!key)
        return;
    current = env_list;
    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            new_value = value ? strdup(value) : strdup("");
            if (!new_value)
                return;
            free(current->value);
            current->value = new_value;
            return;
        }
        current = current->next;
    }
    new_env = malloc(sizeof(t_env));
    if (!new_env)
        return;
    new_env->key = strdup(key);
    if (!new_env->key)
    {
        free(new_env);
        return;
    }
    new_env->value = value ? strdup(value) : strdup("");
    if (!new_env->value)
    {
        free(new_env->key);
        free(new_env);
        return;
    }
    new_env->next = env_list;
    env_list = new_env;
}

 
char *expand_variable(char *var_name, char **env)
{
    t_env *current;
    int i;
    int len;
    char *arr;

    current = env_list;
    while (current)
    {
        len=strlen(current->key);
        arr=malloc(len+1);
        strcpy(arr,current->key);
        if (strcmp(arr, var_name) == 0)
            return strdup(current->value);   
        free(arr); 
        current = current->next;
    }
     i = 0;
    while (env[i])
    {
        if (strncmp(env[i], var_name, strlen(var_name)) == 0 && env[i][strlen(var_name)] == '=')
            return strdup(env[i] + strlen(var_name) + 1);   
        i++;
    }
    return NULL;
}
