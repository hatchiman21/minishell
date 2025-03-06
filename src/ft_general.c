/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_general.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:40:38 by aatieh            #+#    #+#             */
/*   Updated: 2025/03/06 10:35:43 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	first_step(char **line, t_minishell *vars)
{
	vars->op_num = 1;
	vars->tmp_fd = -1;
	*line = readline("~/minishell$ ");
	if (!*line)
		return (-1);
	if (g_ctrl_c)
		vars->exit_status = 130;
	add_history(*line);
	if (first_input_check(*line, vars) != -1)
	{
		free(*line);
		vars->exit_status = 2;
		return (1);
	}
	vars->final_line = ft_strdup(*line);
	if (!vars->final_line)
	{
		ft_putstr_fd("minishell: final line malloc failed\n", 2);
		free(*line);
		return (-1);
	}
	return (0);
}

int	final_step(char **line, t_minishell *vars)
{
	int		error;

	error = 0;
	*line = expand_all(vars, *line);
	vars->argc = words_count_sh(*line);
	vars->argv = get_argv(*line, vars);
	free(*line);
	if (!vars->argv)
	{
		ft_free_red(&vars->redirections);
		close_free_here_doc(&vars->here_doc_fds);
		ft_putstr_fd("minishell: argv malloc failed\n", 2);
		return (-1);
	}
	remove_all_qoutes(vars);
	error = process(vars);
	wait_for_all(vars);
	g_ctrl_c = 0;
	close_free_here_doc(&vars->here_doc_fds);
	free_split(&vars->argv, vars->argc);
	ft_free_red(&vars->redirections);
	return (error);
}

void	inti_vars(t_minishell *vars)
{
	vars->exit_status = 0;
	vars->here_doc_fds = NULL;
	vars->redirections = NULL;
	vars->last_id = 0;
	vars->tmp_fd = -1;
	vars->pipefd[0] = -1;
	vars->pipefd[1] = -1;
	vars->line = NULL;
	vars->final_line = NULL;
	vars->argv = NULL;
	vars->argc = 0;
	vars->op_num = 1;
	vars->env = NULL;
	vars->std_in = dup(STDIN_FILENO);
	vars->std_out = dup(STDOUT_FILENO);
	if (vars->std_in == -1 || vars->std_out == -1)
	{
		ft_dprintf(2, "minishell: dup failed\n");
		if (vars->std_in != -1)
			close(vars->std_in);
		exit(EXIT_FAILURE);
	}
}

void	inti_set_up(t_minishell *vars, char **env)
{
	if (!isatty(0))
		exit(EXIT_FAILURE);
	inti_vars(vars);
	vars->env = ft_array_dup(env);
	if (!vars->env && env)
	{
		close_fds((int [3]){vars->std_in, vars->std_out, -1});
		ft_putstr_fd("minishell: enviorment malloc failed\n", 2);
		exit(EXIT_FAILURE);
	}
	increase_shlvl(vars->env);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handle_sigint);
}

char	*expand_all(t_minishell *vars, char *line)
{
	t_redirect	*red;

	red = vars->redirections;
	line = expand(line, *vars);
	if (!line)
	{
		ft_putstr_fd("minishell: line malloc failed\n", 2);
		return (NULL);
	}
	while (red)
	{
		red->content = expand(red->content, *vars);
		red = red->next;
	}
	return (line);
}
