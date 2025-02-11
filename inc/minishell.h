/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yousef <yousef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:32:11 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/11 03:43:45 by yousef           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdlib.h>

typedef struct s_redirect
{
	char				*redirection;
	int					op;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_minishell
{
	int			argc;
	char		**argv;
	char		**env;
	t_redirect	*redirections;
	int			exit_status;
	int			last_id;
	int			op_num;
	int			pipefd[2];
	int			tmp_fd;
}				t_minishell;

char		*expand(char *argv, t_minishell vars);
char		*get_variable(char **env, char *line, int *j, int status);
char		*rev_strdup(char *s, int j);
char		*rm_qoutes(char *line);
char		*dup_without_qoutes(char *s, int counter);
void		ft_free_red(t_redirect *lst);
void		free_split(char **split, int num);
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
void		free_split1(char **res);
void		exit1(char *line, t_minishell vars);
char		**unset(char **env, char *line);
void		my_cd(t_minishell *vars);

#endif
