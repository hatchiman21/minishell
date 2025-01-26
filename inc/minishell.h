/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:32:11 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/27 01:20:31 by aatieh           ###   ########.fr       */
=======
/*   By: yousef <yousef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:32:11 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/26 01:23:04 by yousef           ###   ########.fr       */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:32:11 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/24 20:33:49 by aatieh           ###   ########.fr       */
>>>>>>> 207f4ff9b4f11da43d095e7232dcaa90bde03160
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
	int			pipefd[2];
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
void		free_all(char *str, char **split);

#endif
