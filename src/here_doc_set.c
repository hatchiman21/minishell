/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 14:55:16 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/22 15:51:21 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	here_doc_error_check(char *line, t_here_doc *here_doc_fds)
{
	int	i;

	i = 0;
	if (here_doc_fds)
		return (0);
	if (!line)
		return (0);
	while (line[i])
	{
		skip_qouted_line(line, &i);
		if (line[i] == '<' && line[i + 1] == '<')
			return (1);
		i++;
	}
	return (0);
}

int	here_doc_set(char *line, t_minishell *vars)
{
	g_ctrl_c = 0;
	prepare_here_doc(vars, vars->redirections);
	if (here_doc_error_check(vars->final_line, vars->here_doc_fds))
	{
		if (vars->final_line)
			free(vars->final_line);
		free(line);
		ft_free_red(vars->redirections);
		ft_putstr_fd("minishell: here doc malloc failed\n", 2);
		return (-1);
	}
	free(vars->final_line);
	if (g_ctrl_c)
	{
		free(line);
		close_free_here_doc(&vars->here_doc_fds);
		ft_free_red(vars->redirections);
		return (1);
	}
	return (0);
}
