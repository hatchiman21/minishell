/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 23:09:18 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/24 02:52:09 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	quotes_check(char *line, t_minishell *vars)
{
	int	i;
	int	q_flag;

	i = 0;
	q_flag = 0;
	while (line[i])
	{
		if (line[i] == '"' && !q_flag)
			q_flag = 1;
		else if (line[i] == '\'' && !q_flag)
			q_flag = 2;
		else if ((line[i] == '"' && q_flag == 1)
			|| (line[i] == '\'' && q_flag == 2))
			q_flag = 0;
		i++;
	}
	if (q_flag)
	{
		vars->exit_status = 2;
		ft_dprintf(2, "minishell: %s\n", "syntax error: unclosed quote");
		return (1);
	}
	return (-1);
}

int	inital_pipe_check(char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] != '|')
		return (-1);
	return (i);
}

int	first_input_check(char *line, t_minishell *vars)
{
	int	i;

	i = quotes_check(line, vars);
	if (i != -1)
		return (i);
	i = inital_pipe_check(line);
	if (i == -1)
		i = redirections_error_check(line);
	if (i != -1)
	{
		if (i == (int)ft_strlen(line))
			ft_dprintf(2, "minishell: syntax %s",
				"error near unexpected token `newline'\n");
		else
			ft_dprintf(2, "minishell: %s`%c'\n",
				"syntax error near unexpected token ", line[i]);
		vars->exit_status = 2;
	}
	return (i);
}

int	ambiguous_check(char *line)
{
	int	i;
	int	n;
	int	space_found;

	i = skip_red_sign(line);
	n = i;
	space_found = 0;
	while (line[i])
	{
		skip_qouted_line(line, &i);
		if (line[i] != ' ' && line[i] != '\t' && space_found)
		{
			ft_dprintf(2, "minishell: %s: ambiguous redirect\n", line + n);
			return (1);
		}
		if (!space_found && (line[i] == ' ' || line[i] == '\t'))
			space_found = 1;
		i++;
	}
	return (0);
}
