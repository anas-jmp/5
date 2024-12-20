#include "../../include/minishell.h"


char *check_if_dir(char *cmd_path)
 {
  struct stat info;

  if (!stat(cmd_path, &info) && !S_ISDIR(info.st_mode))
    return (cmd_path);
  else {
    free(cmd_path);
    return NULL;
  }
}

char *cmd_helper(char *cmd, char *PATH_VARIABLE) {
  char *cmd_path;
  char **potencial_paths;
  int i;

  i = 0;
  potencial_paths = ft_split(PATH_VARIABLE, ":");
  while (potencial_paths[i]) {
    cmd_path = ft_strjoin(potencial_paths[i], cmd);
    if (!access(cmd_path, X_OK)) {
      ft_free_split(potencial_paths);
      return check_if_dir(cmd_path);
    } else
      free(cmd_path);
    i++;
  }
  ft_free_split(potencial_paths);
  return (NULL);
}

char *get_command_path(char *cmd, char *PATH_VARIABLE)
{
  char *resp;

  cmd = ft_strjoin("/", cmd);
  resp = cmd_helper(cmd, PATH_VARIABLE);
  free(cmd);
  return (resp);
}
char **handle_env_copy(char **env)
{
    int i = 0;
    // Count the number of environment variables
    while (env[i])
    {
        i++;
    }
    // Allocate memory for the copy of the environment
    char **env_copy = (char **)malloc(sizeof(char *) * (i + 1));
    if (!env_copy) {
        perror("malloc");
        return NULL;  // Handle memory allocation error
    }

    // Copy each environment variable
    for (int j = 0; j < i; j++)
     {
        env_copy[j] = strdup(env[j]);
        if (!env_copy[j]) {
            perror("strdup");
            return NULL;  // Handle memory allocation error
        }
    }

    env_copy[i] = NULL;  // Null-terminate the environment copy

    return env_copy;
}

char *is_command(char *cmd, char **env)
 {
  int i;
  struct stat info;
    // if(env!=NULL)
    // {
      
    //      env[0] = "TERM=xterm-256color";
    //      env[1] = "PATH=/usr/local/bin:/usr/bin:/bin";
    //      env[2] = "HOME=/home/anas_pc";
    //      env[3] = "USER=yanas_pc";
    //      env[4] = NULL;  // Null-terminate the array
    //  }
    // i = 0;
    // while (env[i])
    // {
    //     printf("\n%s\n",env[i]);
    //     i++;
    // } 
  // while (env[i])
  //   {
  //       i++;
  //   }
  //   // Allocate memory for the copy of the environment
  //   char **env_copy = (char **)malloc(sizeof(char *) * (i + 1));
  //   if (!env_copy)
  //   {
  //       perror("malloc");
  //       return NULL;  // Handle memory allocation error
  //   }
  //       // Copy each environment variable
  //   for (int j = 0; j < i; j++)
  //    {
  //       env_copy[j] = strdup(env[j]);
  //       if (!env_copy[j]) {
  //           perror("strdup");
  //           return NULL;  // Handle memory allocation error
  //       }
  //   }
  //     printf("\n---xHOLA---\n");
  //  while (env_copy[i])
  //   {
  //       printf("\n%s\n",env_copy[i]);
  //       i++;
  //   } 
  // if (env == NULL) 
  // {
  //   printf("\n>________<\n");
  //       env = handle_env(env_list);  // Copy the environment if it's NULL
  //       if (!env) {
  //           return NULL;  // Handle error in copying environment
  //       }
  //   }
   i = 0;
  if (ft_memchr(cmd, '/', ft_strlen(cmd))) {
    if (!access(cmd, X_OK) && !stat(cmd, &info) && !S_ISDIR(info.st_mode))
      return (cmd);
    else
      return NULL;
  }
  while (env[i])
  {
    if (!ft_strncmp(env[i], "PATH=", 5))
      return (get_command_path(cmd, env[i] + sizeof(char) * 5));
    i++;
  }
  i=0;
   // printf("\n---HOLA---\n");
  //     while (env_copy[i])
  //   {
  //       printf("\n%s>>\n",env_copy[i]);
  //       i++;
  //   } 
  //return (NULL);
  return get_command_path(cmd, "/bin:/usr/bin");
}
