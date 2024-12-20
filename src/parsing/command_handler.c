#include "../../include/minishell.h"
#include <stdio.h>
#include <string.h>

t_env *env_list = NULL;  // Initialize the custom environment list
 
void split_key_value(char *input, char **key, char **value) 
{
      *key = NULL;
    *value = NULL;
  char *equal_sign = strchr(input, '=');
  if (equal_sign) {
    int key_len = equal_sign - input;
    int val_len = strlen(equal_sign + 1);

    *key = malloc(key_len + 1);
    *value = malloc(val_len + 1);

    if (*key && *value) {
      strncpy(*key, input, key_len);
      (*key)[key_len] = '\0';         // Null-terminate key
      strcpy(*value, equal_sign + 1); // cpy value part after '='
    } else {
      free(*key);
      free(*value);
      *key = NULL;
      *value = NULL;
    }
  } else {
    *key = NULL;
    *value = NULL;
  }
}
 

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

void parse_export_value(const char *input, char **key, char **value)
{
    int     i;
    char    *equals;
    
    *key = NULL;
    *value = NULL;
    if (!input)
        return ;
    equals = strchr(input, '=');
    if (!equals)
    {
        *key = strdup(input);
        return ;
    }
    i = equals - input;
    *key = malloc(i + 1);
    if (!*key)
        return ;
    i = 0;
    while (input[i] && input + i < equals)
    {
        (*key)[i] = input[i];
        i++;
    }
    (*key)[i] = '\0';
    
    equals++;
    if (*equals == '"')
    {
        equals++;
        i = 0;
        while (equals[i] && equals[i] != '"')
            i++;
        *value = malloc(i + 1);
        if (!*value)
        {
            free(*key);
            *key = NULL;
            return ;
        }
        i = 0;
        while (equals[i] && equals[i] != '"')
        {
            (*value)[i] = equals[i];
            i++;
        }
        (*value)[i] = '\0';
    }
    else
    {
        *value = strdup(equals);
        if (!*value)
        {
            free(*key);
            *key = NULL;
        }
    }
}

void store_env_variable(char *key, char *value)
{
    t_env   *current;
    t_env   *new_env;
    char    *new_value;

    if (!key)
        return ;
    current = env_list;
    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            new_value = value ? strdup(value) : strdup("");
            if (!new_value)
                return ;
            free(current->value);
            current->value = new_value;
            return ;
        }
        current = current->next;
    }
    new_env = malloc(sizeof(t_env));
    if (!new_env)
        return ;
    new_env->key = strdup(key);
    if (!new_env->key)
    {
        free(new_env);
        return ;
    }
    new_env->value = value ? strdup(value) : strdup("");
    if (!new_env->value)
    {
        free(new_env->key);
        free(new_env);
        return ;
    }
    new_env->next = env_list;
    env_list = new_env;
}

void handle_export_command(char *input)
{
    int     i;
    char    *key;
    char    *value;
    char    *assignment;

    if (!input)
        return ;
    i = 0;
    while (input[i] && input[i] != ' ')
        i++;
    while (input[i] && input[i] == ' ')
        i++;
    assignment = input + i;
    if (!*assignment)
        return ;
    parse_export_value(assignment, &key, &value);
    if (key && is_valid_identifier(key))
        store_env_variable(key, value);
    else if (assignment[0])
        write(2, "minishell: export: not a valid identifier\n", 40);
    if (key)
        free(key);
    if (value)
        free(value);
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

// int is_valid_identifier(const char *key) 
// {
//      int i;

//     if (!key || !key[0])
//         return 0;  

//      if (key[0] == '\'' || key[0] == '"' || strchr(key, '\'') || strchr(key, '"'))
//         return 0;

//     if (!isalpha(key[0]) && key[0] != '_')
//         return 0;

//    i = 1;
//     while (key[i]) 
//     {
//         if (!isalnum(key[i]) && key[i] != '_')
//             return 0;
//       i++;
//     }

//     return 1;
// }


// void parse_export_value(const char *input, char **key, char **value)
// {
//     if (!input || !key || !value)
//     {
//         return;
//     }
    
//      char *equals = strchr(input, '=');
//     if (!equals) {
//         *key = strdup(input);
//         *value = NULL;
//         return;
//     }
//         size_t key_len = equals - input;
//     *key = malloc(key_len + 1);
//     if (!*key) {
//         return;
//     }
//     strncpy(*key, input, key_len);
//     (*key)[key_len] = '\0';
    
//      const char *val_start = equals + 1;
    
//      if (!*val_start) {
//         *value = strdup("");
//         return;
//     }
    
//      if (*val_start == '"') {
//         // Find closing quote
//         const char *closing_quote = strrchr(val_start + 1, '"');
//         if (closing_quote) {
//             // Calculate length excluding quotes
//             size_t val_len = closing_quote - (val_start + 1);
//             *value = malloc(val_len + 1);
//             if (!*value) {
//                 free(*key);
//                 *key = NULL;
//                 return;
//             }
//              strncpy(*value, val_start + 1, val_len);
//             (*value)[val_len] = '\0';
//         } else {
//              *value = strdup(val_start);
//         }
//     } else {
//          *value = strdup(val_start);
//     }
    
//      if (!*value) {
//         free(*key);
//         *key = NULL;
//     }
// }

// void store_env_variable(char *key, char *value) {
//     t_env *current = env_list;
    
//     if (!key) {
//         printf("Error: key is NULL\n");
//         return;
//     }
    
//      if (!value) {
//         value = "";
//     }
    
//      while (current) {
//         if (strcmp(current->key, key) == 0) {
//             char *new_value = strdup(value);
//             if (!new_value) {
//                 printf("Error: malloc failed\n");
//                 return;
//             }
//             free(current->value);
//             current->value = new_value;
//             return;
//         }
//         current = current->next;
//     }
    
//      t_env *new_env = malloc(sizeof(t_env));
//     if (!new_env) {
//         printf("Error: malloc failed\n");
//         return;
//     }
    
//     new_env->key = strdup(key);
//     if (!new_env->key) {
//         free(new_env);
//         printf("Error: malloc failed\n");
//         return;
//     }
    
//     new_env->value = strdup(value);
//     if (!new_env->value) {
//         free(new_env->key);
//         free(new_env);
//         printf("Error: malloc failed\n");
//         return;
//     }
    
//     new_env->next = env_list;
//     env_list = new_env;
// }
// void handle_export_command(char *input)
// {
//     size_t pos = 0;
//     char *token;
//     char *key;
//     char *value;

//      while (input[pos] && !isspace(input[pos]))
//         pos++;
//     while (input[pos] && isspace(input[pos]))
//         pos++;

//      char *assignment = input + pos;
//     if (!*assignment)
//         return;
//     parse_export_value(assignment, &key, &value);
//     if (key && is_valid_identifier(key)) {
//         store_env_variable(key, value);
//     } else {
//         printf("minishell: export: `%s': not a valid identifier\n", assignment);
//     }
    
//     free(key);
//     free(value);
// }