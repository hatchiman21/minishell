/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_checks.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:04:53 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/19 21:48:41 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	variable_has_space(char *line, char **env)
{
	int	variable_len;
	int	n;

	variable_len = 0;
	n = 0;
	while (!word_end(line[variable_len]))
		variable_len++;
	while (env[n])
	{
		if (!ft_strncmp(line, env[n], variable_len))
			break ;
		n++;
	}
	if (!env[n])
		return (false);
	if (ft_strchr(env[n], ' '))
	{
		line[variable_len] = '\0';
		ft_dprintf(2, "minishell: %s: ambiguous redirect\n", line - 1);
		return (true);
	}
	return (false);
}

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

void	redirections_error_check_helper(int *i, char *line, int *red, int *pipe)
{
	if (line[*i] != '>' && line[*i] != '>' && line[*i] != ' ' && (*red))
		(*red) = 0;
	else if ((line[*i] == '>' || line[*i] == '<')
		&& (line[*i + 1] == '>' || line[*i + 1] == '<'))
	{
		(*i)++;
		(*red)++;
	}
	else if (line[*i] == '>' || line[*i] == '<')
		(*red)++;
	else if (line[*i] != '|' && line[*i] != ' ' && (*pipe))
		(*pipe) = 0;
	else if (line[*i] == '|')
		(*pipe) = 1;
}

void	skip_qouted_line(char *line, int *i)
{
	char	quote;

	if (line[*i] == '\'' || line[*i] == '"')
		quote = line[*i];
	else
		return ;
	(*i)++;
	while (line[*i] != quote)
		(*i)++;
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
		skip_qouted_line(line, &i);
		if ((line[i] == '>' || line[i] == '<' || line[i] == '|') && red)
			return (i);
		else if (line[i] == '|' && pipe)
			return (i);
		else if (((line[i] == '>' && line[i + 1] == '<')
			|| (line[i] == '<' && line[i + 1] == '>')))
			return (i + 1);
		else
			redirections_error_check_helper(&i, line, &red, &pipe);
		i++;
	}
	if (red || pipe)
		return (i);
	return (0);
}
