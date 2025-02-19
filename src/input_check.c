/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 23:09:18 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/19 21:37:13 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	quotes_check(char *line)
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
		ft_dprintf(2, "minishell: %s\n", "syntax error: unclosed quote");
		return (1);
	}
	return (0);
}

int	first_input_check(char *line)
{
	int	i;

	i = quotes_check(line);
	if (i)
		return (i);
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

int	ambiguous_check(char *line, char **env)
{
	int	i;
	int	q_flag;

	i = 0;
	q_flag = 0;
	while (line[i] == '>' || line[i] == '<')
		i++;
	while (line[i])
	{
		if (line[i] == '"' && !q_flag)
			q_flag = 1;
		else if (line[i] == '\'' && !q_flag)
			q_flag = 2;
		else if ((line[i] == '"' && q_flag == 1)
			|| (line[i] == '\'' && q_flag == 2))
			q_flag = 0;
		if (line[i] == '$' && !q_flag && variable_has_space(line + i + 1, env))
			return (1);
		i++;
	}
	return (0);
}

char	*remove_edge_spaces(char *line)
{
	int	i;
	int	j;

	i = ft_strlen(line) - 1;
	while (i >= 0 && line[i] == ' ')
	{
		line[i] = '\0';
		i--;
	}
	i = 0;
	j = 0;
	while (line[i] == ' ')
		i++;
	while (line[i])
	{
		line[j] = line[i];
		i++;
		j++;
	}
	line[j] = '\0';
	return (line);
}

int	ambiguous_redirect_check(t_redirect *red, char **env)
{
	while (red)
	{
		red->redirection = remove_edge_spaces(red->redirection);
		if (ambiguous_check(red->redirection, env))
			return (1);
		red = red->next;
	}
	return (0);
}
