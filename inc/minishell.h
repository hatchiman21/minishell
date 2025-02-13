/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:32:11 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/14 13:14:35 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>

typedef struct s_redirect
{
	char				*redirection;
	int					op;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_here_doc
{
	int					fd;
	int					red_order;
	bool				open;
	struct s_here_doc	*next;
}						t_here_doc;

typedef struct s_minishell
{
	int			argc;
	char		**argv;
	char		**env;
	t_redirect	*redirections;
	char		*final_line;
	int			exit_status;
	int			last_id;
	int			op_num;
	int			pipefd[2];
	int			tmp_fd;
	t_here_doc	*here_doc_fds;
}				t_minishell;

char		*expand(char *argv, t_minishell vars);
char		*get_variable(char **env, char *line, int *j,
				int status);
char		*rev_strdup(char *s, int j);
char		*rm_qoutes(char *line);
char		*dup_without_qoutes(char *s, int counter);
int			first_input_check(char *line);
void		error_msg(char *msg);
char		**unset(char **env, char *line);
void		my_cd(t_minishell *vars);
int			ambiguous_redirect_check(t_redirect *red, char **env);
int			word_end(char c);
void		prepare_here_doc(t_minishell *vars, t_redirect *red);
int			get_here_doc_fd(t_here_doc *here_doc, int red_order);
void		ft_echo(char **argv);
void		close_free_here_doc(t_here_doc **here_doc);
int			redirections_error_check(char *line);
bool		variable_has_space(char *line, char **env);
void		*ft_free_red(t_redirect *lst);
t_redirect	*get_redirections(char *line);
void		redirectionadd_back(t_redirect **lst, t_redirect *new);
int			words_count_sh(char *line);
char		*get_token_sh(char *line, int *j, t_minishell *vars);
char		**get_argv(char *line, t_minishell *vars);
char		*ft_merge(char *s1, char *s2, int free_s1, int free_s2);
void		remove_from_line(char *line, int i, int j);
int			word_check(char *line, int i);
void		free_all(char *str, char **split);
char		*get_path(char **cmd, char **envp);
void		process(t_minishell *vars);
void		wait_for_all(t_minishell *vars);
char		**export(char **env, char *line);
void		sig_stay(void);
void		sig_nothing(void);
void		handle_sigquit(int sig);
void		handle_sigint(int sig);
void		exit1(char *state, t_minishell *vars);
void		ft_excute(char *path, char **cmd, t_minishell *vars);
void		env(char **env);
void		pwd(void);

#endif
