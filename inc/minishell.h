/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:32:11 by aatieh            #+#    #+#             */
/*   Updated: 2025/04/26 12:06:35 by aatieh           ###   ########.fr       */
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
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/ioctl.h>

extern int	g_ctrl_c;

typedef struct s_redirect
{
	char				*content;
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
	char		*line;
	char		*final_line;
	int			exit_status;
	int			last_id;
	int			op_num;
	int			pipefd[2];
	int			tmp_fd;
	int			std_in;
	int			std_out;
	t_here_doc	*here_doc_fds;
}				t_minishell;

int			here_doc_set(char *line, t_minishell *vars);
int			first_step(char **line, t_minishell *vars);
int			final_step(char **line, t_minishell *vars);
void		inti_set_up(t_minishell *vars, char **env);
char		*expand_all(t_minishell *vars, char *line);

void		handle_sigint(int sig);
void		remove_all_qoutes(t_minishell *vars);
char		**ft_array_dup(char **array);
int			word_check(char *line, int i);
void		increase_shlvl(char **env);
int			array_size(char **array);
void		close_fds(int *fd);

char		*expand(char *line, t_minishell vars);
char		*get_variable(t_minishell *vars, char *line, int *j);
char		*rev_strdup(char *s, int j, t_minishell *vars);
char		*rm_qoutes(char *line);
int			first_input_check(char *line, t_minishell *vars);
int			write_expanded_line(int fd, char *line, int i, t_minishell *vars);
int			words_count_sh(char *line);
char		**get_argv(char *line, t_minishell *vars);
char		*ft_merge(char *s1, char *s2, int free_s1, int free_s2);
int			word_check(char *line, int i);

void		here_doc_addback(t_here_doc **head, t_here_doc *new);
void		prepare_here_doc(t_minishell *vars, t_redirect *red);
int			get_here_doc_fd(t_here_doc *here_doc, int red_order);
void		close_free_here_doc(t_here_doc **here_doc);

char		*get_path(char **cmd, char **envp, t_minishell *vars);
int			open_file(t_minishell *vars, t_redirect *red,
				int red_order);
int			built_in_fn(char **cmd, t_minishell *vars);
int			process(t_minishell *vars);

int			intial_red(t_minishell *vars, int *cur_op);
int			apply_redirection(t_minishell *vars, int cur_op);
int			here_doc_set(char *line, t_minishell *vars);
int			skip_red_sign(char *line);

int			word_end(char c);
t_redirect	*get_redirections(char *line);
int			redirections_error_check(char *line);
int			ambiguous_redirect_check(t_redirect *red, char **env);
int			ambiguous_check(char *line);
void		remove_from_line(char *line, int i, int j);
int			cmd_built_in(char **cmd);

void		wait_for_all(t_minishell *vars);
void		*ft_free_red(t_redirect **lst);
void		ft_free(t_minishell *vars);
void		skip_qouted_line(char *line, int *i);
void		cmd_error_handling(t_minishell *vars);

void		my_cd(char **argv, char **env, t_minishell *vars);
int			ft_echo(char **argv);
void		ft_exit(char **cmd, t_minishell *vars);
char		**unset(char **env, char **line, t_minishell *vars);
char		**export(char **env, char **line, t_minishell *vars, int t);	
void		export3(char **env, t_minishell *vars, int len);
int			env(char **env, char **cmd);
int			pwd(void);

#endif