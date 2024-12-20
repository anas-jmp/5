#include "../../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_REDIRECT_SIZE 4 // Initial size for redirection arrays
#define INITIAL_ARGS_SIZE 16

 void free_command(t_command *cmd)
 {
  if (cmd) 
  {
    free(cmd->command);
    if (cmd->args)
    {
      for (int i = 0; cmd->args[i]; i++)
      {
        free(cmd->args[i]);
      }
      free(cmd->args);
    }
    if (cmd->input_redirect) 
    {
      for (int i = 0; cmd->input_redirect[i]; i++)
      {
        free(cmd->input_redirect[i]);
      }
      free(cmd->input_redirect);
    }
    if (cmd->output_redirect)
    {
      for (int i = 0; cmd->output_redirect[i]; i++)
      {
        free(cmd->output_redirect[i]);
      }
      free(cmd->output_redirect);
    }
    free(cmd->append_modes);
    free(cmd);
  }
}

// Helper function to resize an int array (for append_modes)
int *resize_int_array(int *old_array, int old_size, int new_size)
{
  int *new_array = malloc(new_size * sizeof(int));
  if (!new_array) {
    return NULL;
  }

  // Copy old array content to new array
  for (int i = 0; i < old_size; i++) 
  {
    new_array[i] = old_array[i];
  }

  // Free old array
  free(old_array);

  return new_array;
}

char **resize_array(char **old_array, int old_size, int new_size) 
{
  char **new_array = malloc(new_size * sizeof(char *));
  if (!new_array) {
    return NULL;
  }
  // Copy old array content to new array
  for (int i = 0; i < old_size; i++) {
    new_array[i] = old_array[i];
  }

   free(old_array);

  return new_array;
}
extern t_env *env_list;

char **resize_args(char **old_args, int old_size, int new_size)
 {
  int i = 0;
  char **new_args = malloc(new_size * sizeof(char *));
  if (!new_args)
    return NULL;

  while (i < old_size) {
    new_args[i] = old_args[i]; // Copy over the old arguments
    i++;
  }

  free(old_args);  // Free the old array
  return new_args; // Return the new and resized array
}
 