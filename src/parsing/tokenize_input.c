#include "../../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_REDIRECT_SIZE 4 // Initial size for redirection arrays
#define INITIAL_ARGS_SIZE 16



void add_token(token **head, token **current, char *value, token_type type)
{
     token *new_token = malloc(sizeof(token));
    if (!new_token) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    new_token->value = value;
    new_token->token_type = type;
    new_token->next = NULL;

     if (!*head) 
     {
        *head = new_token;
    } else {
        (*current)->next = new_token;
    }
    *current = new_token;
}

token *tokenize_input(const char *input, char **env)
{
  token *head ;
  token *current;
  const char *str;

    current = NULL;
    str = input;    
    head = NULL;
  if (!str)
      return NULL;
 while (*str)
 {
         while (isspace(*str))
            str++;
        if (*str == '\0')
            break;
        char *token_value = NULL;
        token_type type;
         token_value = next_token(&str, env, &type);
        if (token_value == NULL)
            continue; 
         add_token(&head, &current, token_value, type);
  }
  return head;
}