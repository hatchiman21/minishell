/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:32:11 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/24 19:40:31 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

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

void	expand(char **argv, t_minishell vars);
void	get_env(char **env);
void	get_pwd(char **env);
void	expand(char **argv, t_minishell vars);
char	*get_variable(char **env, char *line, int *j);
char	*rev_strdup(char const *s, int j);
char	*rm_qoutes(char *line);
char	*dup_without_qoutes(char *s, int counter);
void	ft_free_lst(t_redirect *lst);
void	free_split(char **split, int num);
t_redirect	*get_redirections(char *line);
void	redirectionadd_back(t_redirect **lst, t_redirect *new);
int		test_expander(int argc, char **argv, char **env);

#endif