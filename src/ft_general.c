/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_general.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:40:38 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/16 01:32:55 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	here_doc_set(char *line, t_minishell *vars)
{
	prepare_here_doc(vars, vars->redirections);
	if (!vars->here_doc_fds
		&& ft_strnstr(vars->final_line, "<<", ft_strlen(vars->final_line)))
	{
		if (vars->final_line)
			free(vars->final_line);
		free(line);
		ft_free_red(vars->redirections);
		ft_putstr_fd("minishell: here doc malloc failed\n", 2);
		return (-1);
	}
	add_history(vars->final_line);
	free(vars->final_line);
	return (0);
}

int	first_step(char **line, t_minishell *vars)
{
	vars->op_num = 1;
	*line = readline("~/minishell$ ");
	if (!*line)
		return (-1);
	if (first_input_check(*line))
	{
		add_history(*line);
		free(*line);
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
	*line = expand_all(vars, *line);
	vars->argc = words_count_sh(*line);
	vars->argv = get_argv(*line, vars);
	free(*line);
	if (!vars->argv)
	{
		ft_free_red(vars->redirections);
		ft_putstr_fd("minishell: argv malloc failed\n", 2);
		return (-1);
	}
	remove_all_qoutes(vars);
	process(vars);
	close_free_here_doc(&vars->here_doc_fds);
	free_split(vars->argv, vars->argc);
	ft_free_red(vars->redirections);
	return (0);
}

void	inti_set_up(t_minishell *vars, char **env)
{
	if (!isatty(0))
		exit (EXIT_FAILURE);
	vars->env = ft_array_dup(env);
	if (!vars->env && env)
	{
		ft_putstr_fd("minishell: enviorment malloc failed\n", 2);
		exit (EXIT_FAILURE);
	}
	increase_shlvl(vars->env);
	vars->exit_status = 0;
	vars->here_doc_fds = NULL;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handle_sigint);
	vars->std_in = dup(STDIN_FILENO);
	vars->std_out = dup(STDOUT_FILENO);
}

char	*expand_all(t_minishell *vars, char *line)
{
	t_redirect	*red;

	red = vars->redirections;
	line = expand(line, *vars);
	while (red)
	{
		red->redirection = expand(red->redirection, *vars);
		red = red->next;
	}
	return (line);
}
