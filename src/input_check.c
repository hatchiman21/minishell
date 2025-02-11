/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 23:09:18 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/11 17:42:08 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_calculate_flag(t_redirect *red, int i, int q_flag)
{
	if (red->redirection[i] == '"' && !q_flag)
		q_flag = 1;
	else if (red->redirection[i] == '\'' && !q_flag)
		q_flag = 2;
	else if ((red->redirection[i] == '"' && q_flag == 1)
		|| (red->redirection[i] == '\'' && q_flag == 2))
		q_flag = 0;
	return (q_flag);
}

int	redirections_validity_check(t_redirect *redirection)
{
	t_redirect	*red;
	int			i;
	int			q_flag;

	red = redirection;
	while (red)
	{
		i = 0;
		q_flag = 0;
		while (red->redirection[i] == '>' || red->redirection[i] == '<')
			i++;
		while (red->redirection[i])
		{
			q_flag = ft_calculate_flag(red, i, q_flag);
			if (red->redirection[i] == ' ' && !q_flag)
				return (1);
			i++;
		}
		red = red->next;
	}
	return (0);
}

int	redirections_error_check(char *line)
{
	int	i;
	int	red;
	int	pipe;

	i = 0;
	red = 0;
	pipe = 0;
	while (line[i])
	{
		if ((line[i] == '>' || line[i] == '<' || line[i] == '|') && red)
			return (i);
		else if (line[i] == '|' && pipe)
			return (i);
		else if (line[i] != '>' && line[i] != '>' && line[i] != ' ' && red)
			red = 0;
		else if (line[i] != '|' && line[i] != ' ' && pipe)
			pipe = 0;
		else if (line[i] == '|')
			pipe = 1;
		else if ((line[i] == '>' && line[i + 1] == '<')
			|| (line[i] == '<' && line[i + 1] == '>'))
			return (i + 1);
		else if ((line[i] == '>' || line[i] == '<')
			&& (line[i + 1] == '>' || line[i + 1] == '<'))
		{
			i++;
			red++;
		}
		else if (line[i] == '>' || line[i] == '<')
			red++;
		i++;
	}
	if (red || pipe)
		return (i);
	return (0);
}

int	first_input_check(char *line)
{
	int	i;

	i = redirections_error_check(line);
	if (i)
	{
		if (i == (int)ft_strlen(line))
			ft_dprintf(2, "minishell: syntax %s",
				"error near unexpected token `newline'\n");
		else
			ft_dprintf(2, "minishell: %s`%c'\n",
				"syntax error near unexpected token ", line[i]);
	}
	return (i);
}
