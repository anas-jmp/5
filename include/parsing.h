# ifndef MINI_SHELL_H
# define MINI_SHELL_H
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <fcntl.h>
# include <sys/stat.h>
#include <ctype.h>  
#include <stdio.h>
#include <stdbool.h>
#define IS_STRING_TOKEN(type) ((type) == WORD || (type) == DOUBLE_QUOTED_STRING || (type) == SINGLE_QUOTED_STRING)
 // token.h

typedef enum {
    WORD,
    EXPANDABLE,
    INPUT_REDIRECTION,
    OUTPUT_REDIRECTION,
    OUTPUT_REDIRECTION_APPEND_MODE,
    PIPES,
    SINGLE_QUOTED_STRING,
    DOUBLE_QUOTED_STRING,
    HERE_DOC
} token_type;

typedef struct s_parser_state {
    const char **str; // Input string pointer
    char **env;       // Environment variables
    int status;       // Status for variable expansion (e.g., last exit code)
} t_parser_state;

typedef struct token 
{
    char *value;
    token_type token_type;
    struct token *next;
} token;

typedef struct s_counters
{
    int arg_count;
    int max_args;
    int input_count;
    int max_inputs;
    int output_count;
    int max_outputs;
    int heredoc_count;
    int max_heredocs;
} t_counters;

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;
typedef struct s_count
{
    int max_args;
    int max_inputs;
    int max_outputs;
    int max_heredocs;
} t_count;

typedef struct s_shell_state
 {
    int exit_status;
    int state;
    int prompt_redrawn;
} t_shell_state;
 
 typedef struct s_token_data 
{
    char quote;
    const char *quoted_start;
    size_t len;
    char *token_value;
    char *expanded_token;
} t_token_data;

typedef struct s_parser 
{
    token *tokens;         // Pointer to the current token being processed
    t_command *head;       // Head of the linked list of commands
    t_command *current;    // Current command in the linked list
    int arg_count;         // Count of arguments for the current command
    int input_count;       // Count of input redirections
    int output_count;      // Count of output redirections
    int heredoc_count;     // Count of heredocs
 } t_parser;

 t_command *parse_command(token *tokens);



/**************************today**********************************/
 void free_command(t_command *cmd);
int *resize_int_array(int *old_array, int old_size, int new_size);
char **resize_array(char **old_array, int old_size, int new_size);
char **resize_args(char **old_args, int old_size, int new_size);
char *process_export_command(const char **str, token_type *type);
char *process_unset_command(const char **str, token_type *type);
char *process_echo_command(const char **str, token_type *type);
char *process_unset_command(const char **str, token_type *type);
char *process_echo_command(const char **str, token_type *type);
char *process_export_command(const char **str, token_type *type);
char *next_tok2(const char *start, const char **str, token_type *type);
char *next_tok3(const char **str, char **env, token_type *type);
void process_cursor(const char **cursor, char **res_cursor, char **env);

t_command *init_command();
char *handle_quoted_string_with_expansion(char *input, char **env);
void handle_variable_expansion(const char **cursor, char **res_cursor, char **env);
void copy_regular_characters(const char **cursor, char **res_cursor);
char *allocate_and_duplicate_result(const char *result, char *expanded_input);
char *extract_variable_name(const char **cursor);

void add_token(token **head, token **current, char *value, token_type type);
token *tokenize_input(const char *input, char **env);
char *handle_quoted_string_with_expansion(char *input, char **env);

int handle_input_redirection(t_command *cmd, token **tokens, int *input_count, int *max_inputs);
int handle_output_redirection(t_command *cmd, token **tokens, int *output_count, int *max_outputs);
bool set_command_and_args(token **tokens, t_command *cmd, int *arg_count);
bool handle_single_quoted_string(token **tokens, t_command *cmd, int *arg_count);
int handle_heredoc_redirection(t_command *cmd, token **tokens, int *heredoc_count, int *max_heredocs);
int handle_output_redirection(t_command *cmd, token **tokens, int *output_count, int *max_outputs);
int handle_input_redirection(t_command *cmd, token **tokens, int *input_count, int *max_inputs);
bool set_command_and_args(token **tokens, t_command *cmd, int *arg_count);
bool handle_single_quoted_string(token **tokens, t_command *cmd, int *arg_count);
char *handle_standalone_dollar(char *prefix);
char *combine_prefix_and_expanded(char *prefix, char *expanded);
char *handle_var_expansion(const char **str, const char *current, char **env, token_type *type);
char *get_env_value(const char *key, char **env);
/*************************************************************/
//void handle_exiting_2(char *args,char *arg2);
// int handle_exit(char **args);
// void printing(int status);
void write_exit_status_to_file(int status);

int handle_exit(char **args, t_shell_state *shell_state);
char **handle_env(t_env *env_list);
 void handle_command_name(token **tokens, t_command *cmd);
void append_command(t_command **head, t_command **current, t_command *cmd);
char *next_tok_par(const char *start,const char **str,char **env, token_type *type);
char *next_tok_par1(const char *start,const char **str,char **env,int *status ,token_type *type);
char *next_tok_par2(const char *start,const char **str,char **env,int *status ,token_type *type);
char *next_tok_par3(const char *start,const char **str,char **env,int *status ,token_type *type);
char **resize_array(char **old_array, int old_size, int new_size);
int *resize_int_array(int *old_array, int old_size, int new_size);
void is_exit_with_signal(int *status);
 
//void    is_exit_with_signal(int *status);

// char *handle_echo_command(char *input, char **env);
  char	*ft_itoa(int nb);
// pid_t ending_child_processe(t_command *prompt , char *cmd_path , char **argv , int **pipefd , char **env , int j );
void display_ascii_art(void);
 char *expand_variable(char *var_name, char **env);
int validate_syntax(token *tokens);
void echo_exit_status(const char *input);
int check_unmatched_quotes(char *input);
 char *next_token(const char **str, char **env, token_type *type);
extern t_env *env_list;
extern int global_exit_status;

/******************************************/
char  *get_redirection_file(const char *command);
int   count_env_vars(t_env *env_list);
void  print_env(t_env *env_list);
char  **build_envp_array(t_env *env_list);
void  execute_command_fa(char *command_path, char **argv, t_env *env_list);
void  execute_env_command(t_env *env_list, const char *output_file); 
/**********************************************/
void split_key_value(char *input, char **key, char **value);
void handle_unset(const char *key); 
void handle_export_command(char *input);
void store_env_variable(char *key, char *value);
void free_command(t_command *cmd);
char *handle_quoted_string_with_expansion(char *str, char **env);
char *extract_quoted_string(const char **str, char **env);
void handle_expand_command(char *input, char **env);
/**********************************************/
char	*ft_strjoin(char const *s1, char const *s2);
int     pipex(t_command *prompt , char **env);
void    dup_fds(int old_fd , int new_fd);
void output_redirection(char *file_path);
int redidrect_stdin_to_herdoc(char *delimiter , int heredoc_fd) ;
void execute_command(char *command_path , char **argv , char **env);
void output_error_exit(char *error_msg , int status);
char	*is_command(char *cmd, char **env);
//char ft_cd(char* path);
char	*cmd_helper(char *cmd, char *PATH_VARIABLE);
char	*get_command_path(char *cmd, char *PATH_VARIABLE);
void 	ft_free_split(char **split);
void handle_echo_command(char *input, char **env);
/**** */
// t_command *parse_command(token *tokens, char **env);
int validate_syntax(token *tokens);
void print_parsed_command(t_command *cmd);
void execute_pipeline(t_command *cmd_list);
 
/* Lexer and parser functions */
    typedef int (*redir_func_ptr)(char *token);
// token *tokenizer(char *prompt);
token *tokenizer(char *prompt, char **env);
token *tokenize_input(const char *input, char **env);
/* Function declarations */
void split_key_value(char *input, char **key, char **value);
token_type identify_token_type(char *token_value);
char *extract_quoted_string(const char **str, char **env);
// print_parsed_command
void execute_pipeline(t_command *cmd_list);
// void handle_echo_command(char *input, char **env);
void     free_parsed_command(t_command *cmd);
void     free_token_list(token *head);
void     shell_loop(char **env) ;
void     print_tokens(token *head);
void     free_token_list(token *head);
token    *setup_first_node(char **split_prompt, redir_func_ptr *redirection_functions);
void     setup_nodes(char **split_prompt, redir_func_ptr *redirection_functions, token **first_node, int i);
char	 **ft_split(const char *str, char *delimiters);


int     is_INPUT_REDIRECTION(char *token);
int     is_OUTPUT_REDIRECTION_APPEND_MODE(char *token);
int     is_OUTPUT_REDIRECTION(char *token);
int     is_PIPES(char *token);
int     is_WORD(char *token);

 
// Parse tokens into a t_command structure
// t_command *parse_command(token *tokens, char **env);
// Resize the arguments array dynamically
char **resize_args(char **args, int current_size, int new_size);
// Free memory associated with a t_command structure
void free_command(t_command *cmd);
// Handle input redirection, output redirection, and append mode
void handle_redirections(t_command *cmd);
// Debugging utility for tokens (optional)
void print_tokens(token *head);

#endif