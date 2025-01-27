/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:32:11 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/27 03:52:51 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   Updated: 2025/01/24 20:33:49 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
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
	int			last_id;
	int			op_num;
}				t_minishell;

char		*expand(char *argv, t_minishell vars);
void		get_env(char **env);
void		get_pwd(char **env);
char		*get_variable(char **env, char *line, int *j);
char		*rev_strdup(char *s, int j);
char		*rm_qoutes(char *line);
char		*dup_without_qoutes(char *s, int counter);
void		ft_free_lst(t_redirect *lst);
void		free_split(char **split, int num);
t_redirect	*get_redirections(char *line);
void		redirectionadd_back(t_redirect **lst, t_redirect *new);
int			test_expander(int argc, char **argv, char **env);
int			words_count_sh(char *line);
char		*get_token_sh(char *line, int *j);
char		**get_argv(char *line, int argc);
char		*ft_merge(char *s1, char *s2, int free_s1, int free_s2);
void		remove_from_line(char *line, int i, int j);
int			word_check(char *line, int i);
void		gets(char *line, char **env, t_minishell vars);
void		export(char **env, char *line);
void		sig_stay(void);
void		sig_nothing(void);
void		handle_sigquit(int sig);
void		handle_sigint(int sig);
void		s_flag_ch(char *line);

#endif
