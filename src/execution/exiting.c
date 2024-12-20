#include "../../include/minishell.h"

 
int read_exit_status_from_file()
{
    FILE *file = fopen("exit_status.txt", "r");
    int status = 0; // Default value for status
    if (file)
    {
        fscanf(file, "%d", &status); // Read the status from the file
        fclose(file);
    }
    
    else
    {
        // If the file doesn't exist, it means it's the first run, or the file was deleted.
        status = 0; // Default value
    }
    return status;
}


void write_exit_status_to_file(int status)
{
    FILE *file = fopen("exit_status.txt", "w");
    if (file)
    {
        fprintf(file, "%d\n", status); // Write the status to the file
        fclose(file);
    }
    else
    {
        perror("Error writing exit status to file");
    }
}

// void message(int fd, char *status_str,size_t len)
// {
//         if (write(fd, status_str, len) == -1)
//         perror("Error writing to file");
//     if (write(fd, "\n", 1) == -1)
//         perror("Error writing newline to file");
// }
// void write_exit_status_to_file(int status)
// {
//     size_t len;
//     char *status_str;
//     int fd;

//     fd = open("exit_status.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1)
//     {
//         perror("Error opening file");
//         return;
//     }
//      status_str  = ft_itoa(status);
//     if (!status_str)
//     {
//         perror("Error allocating memory for status string");
//         close(fd);
//         return;
//     }
//     len = 0;
//     while (status_str[len] != '\0')
//         len++;
//     message(fd,status_str,len);
//     free(status_str);
//     if (close(fd) == -1)
//         perror("Error closing file");
// }


 
int is_numeric(const char *str)
{
    if (!str || *str == '\0') 
        return 0;  

    if (*str == '-' || *str == '+') 
        str++;  
    while (*str)
    {
        if (!isdigit(*str)) 
            return 0; 
        str++;
    }
    return 1; // All characters are numeric
}

int handle_exit(char **args, t_shell_state *shell_state)
 {
  
    int nbr = 0;
    char *num = NULL;

    printf("\n(%s)\n", args[0]);

    if (args[1] != NULL && is_numeric(args[1]) == 1) {
        nbr = ft_atoi(args[1]);
    }

    printf("\n/********************************/\n");
    printf("arg  1 ---------->%s", args[1]);
    printf("\narg 2 ---------->%s", args[2]);
    printf("\n/********************************/\n");

    if (is_numeric(args[1]) && args[2] == NULL) {
        nbr = ft_atoi(args[1]);
        printf("\nthe number is %d\n", nbr);
        nbr %= 256;
        if (nbr < 0) {
            nbr += 256; // Handle negative values
        }
        shell_state->exit_status = nbr;
        num = ft_itoa(nbr);
        // store_env_variable("?", num);
        write_exit_status_to_file(nbr);
        free(num); // Free allocated memory
        return nbr;
    }

    if (args[1] == NULL && args[2] == NULL) {
        // No arguments provided: Default exit value 0
        shell_state->exit_status = 0;
        // store_env_variable("?", "0");
        printf("\nthe return is 0\n");
        write_exit_status_to_file(0);
        return 0;
    }

    if (is_numeric(args[1]) == 0 && args[1] != NULL) 
    {
        printf("\nbash: exit: %s: numeric argument required\n", args[1]);
        shell_state->exit_status = 2;
        // store_env_variable("?", "2");
        printf("\nthe return is 2\n");
        write_exit_status_to_file(2);
        return 2;
    } else if (is_numeric(args[1]) && args[2] != NULL) {
        printf("\nbash: exit: too many arguments\n");
        // store_env_variable("?", "1");
        write_exit_status_to_file(1);
        shell_state->exit_status = 1;
        printf("\nthe return is 1\n");
        return 1;
    }

    return 0;
}

 
// void printing(t_shell_state *shell_state)
//  {
//     char *nb = ft_itoa(shell_state->exit_status);
//     printf("\nshell_state->exit_status : {%d}\n",shell_state->exit_status);
//     printf("\nstate : <-{%d}->\n",state);
//     // if (state == 1)
//     // {
//     //     store_env_variable("?", "1");
//     //     state = 0;
//     // }

//         store_env_variable("?", nb);
//         // printf("\ndbg <%s>\n", nb);
//         // write_exit_status_to_file(0);
//     free(nb);  
// }



// void printing(t_shell_state *shell_state)
//  {
//     char *nb = ft_itoa(shell_state->exit_status);
//     printf("\nshell_state->exit_status : {%d}\n",shell_state->exit_status);
//     printf("\nstate : <-{%d}->\n",state);
//     if (state == 1)
//     {
//         store_env_variable("?", "1");
//         state = 0;
//     }
//     if (state != -1)
//     {
//         store_env_variable("?", nb);
//         printf("\ndbg <%s>\n", nb);
//         write_exit_status_to_file(0);
//     } 
//     else 
//     {
//         state = 0;
//     }

//     free(nb);  
// }
