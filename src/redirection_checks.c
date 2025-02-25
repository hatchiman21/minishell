/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_checks.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:04:53 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/25 23:43:15 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	redirections_error_check_helper(int *i, char *line, int *red, int *pipe)
{
	if (line[*i] != '>' && line[*i] != '>' && line[*i] != ' '
		&& line[*i] != '\t' && (*red))
		(*red) = 0;
	else if ((line[*i] == '>' || line[*i] == '<')
		&& (line[*i + 1] == '>' || line[*i + 1] == '<'))
	{
		(*i)++;
		(*red)++;
	}
	else if (line[*i] == '>' || line[*i] == '<')
		(*red)++;
	if (line[*i] != '|' && line[*i] != ' ' && line[*i] != '\t' && (*pipe))
		(*pipe) = 0;
	else if (line[*i] == '|')
		(*pipe) = 1;
}

void	skip_qouted_line(char *line, int *i)
{
	char	quote;

	if (line[*i] && (line[*i] == '\'' || line[*i] == '"'))
		quote = line[*i];
	else
		return ;
	(*i)++;
	while (line[*i] && line[*i] != quote)
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
		if (line[i] == '|' && pipe)
			return (i);
		if ((line[i] == '>' || line[i] == '<' || line[i] == '|') && red)
			return (i);
		if (((line[i] == '>' && line[i + 1] == '<')
				|| (line[i] == '<' && line[i + 1] == '>')))
			return (i + 1);
		redirections_error_check_helper(&i, line, &red, &pipe);
		i++;
	}
	if (red || pipe)
		return (i);
	return (-1);
}
