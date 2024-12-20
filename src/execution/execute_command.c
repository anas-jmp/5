#include "../../include/minishell.h"

void execute_command(char *command_path, char **argv, char **env) 
{

  // int i=0;
  // while (*argv)
  // {
  //   printf("\ncommand is (%s)  \n",argv[i]);
  //   i++;
  // }
  
  if (execve(command_path, argv, env) < 0)
    output_error_exit("command", 127);
}